#include "headers/database.h"

Database::Database(int num_threads, string db_path, string config_path, string log_path, TransactionMode trans_mode) : thread_pool(num_threads), num_threads(num_threads) {
    if (trans_mode == ENABLED) {
        db = new database(cstr(DB_NAME), cstr(config_path), DB_OPTIMISTIC_TRANSACTION, cstr(db_path), cstr(log_path));
    } else {
        db = new database(cstr(DB_NAME), cstr(config_path), DB_TRANSACTION_NONE, cstr(db_path), cstr(log_path));
    }
    tbl = b_tbl = NULL;
}

void Database::close_database() {
    if (db) {
        thread_pool.join();
        close_table();
        db->closedatabase();
        delete tbl;
        delete b_tbl;
        delete db;
        db = NULL; tbl = b_tbl = NULL;
    }
}

void Database::use_table(int id) {
    close_table();
    tbl = db->gettable(cstr(TABLE_PREFIX + istr(id)), OPENCREATE, NULL);
    b_tbl = db->gettable(cstr(TABLE_BLACKLIST), OPENCREATE, NULL);
}

void Database::close_table() {
    if (tbl) {
        tbl->closetable();
        b_tbl->closetable();
    }
}

table_env* Database::get_table_properties(bool write_mode) {
    table_env* properties = new table_env();
    return properties;
}

void Database::blacklist_key(FDT* key, connection* conn, connection* b_conn) {
    bool is_blacklisted = true;
    FDT* val = new FDT(&is_blacklisted, sizeof(bool));
    b_conn->put(key, val, INSERT_UNIQUE);
}

void Database::put(key_value data) {
    key_value_map v;
    v[data.first].push_back(data.second);
    thread_pool.enqueue([](Database * db, key_value_map * data, key_value_map::iterator start, key_value_map::iterator end) {
        db->batch_put(data, start, end);
        return 0;
    }, this, &v, v.begin(), v.end()).get();
}

void Database::put(key_value_map& data) {
    ll map_size = data.size();
    ll batch_size = ceil((1.0 * map_size) / num_threads);
    key_value_map::iterator start, end;
    start = end = data.begin();
    advance(end, batch_size);
    vector< std::future<int> > results;
    for (int i = 0; i < num_threads; i++) {
        results.emplace_back(
        thread_pool.enqueue([](Database * db, key_value_map * data, key_value_map::iterator start, key_value_map::iterator end) {
            db->batch_put(data, start, end);
            return 0;
        }, this, &data, start, end));
        map_size = map_size > batch_size ? map_size - batch_size : 0 ;
        advance(start, map_size == 0 ? 0 : batch_size);
        advance(end, min(map_size, batch_size));
    }
    for (auto && result : results)
        result.get();
}

void Database::batch_put(key_value_map* data, key_value_map::iterator start, key_value_map::iterator end) {
    connection* conn = tbl->getconnection();
    connection* b_conn = b_tbl->getconnection();
    for (key_value_map::iterator it = start; it != end; it++) {
        t_key m_key = it->first;
        size_t val_length = (it->second).size();
        FDT* key = new FDT(&m_key, sizeof(t_key));
        FDT* b_value = b_conn->get(key);
        if (!(b_value && *(bool*) (b_value->data))) {
            FDT* o_value = conn->get(key);
            FDT* n_value = new FDT();
            t_value* values = NULL;
            int pos = 0;
            if (o_value != NULL) {
                pos = o_value->length / sizeof(t_value);
                if (pos + val_length >= INDEX_THRESHOLD_LIMIT) {
                    blacklist_key(key, conn, b_conn);
                    continue;
                }
                values = new t_value[pos + val_length];
                copy(values, (t_value*)(o_value->data), pos);
                delete[] (t_value*)(o_value->data);
                delete o_value;
            } else {
                if (val_length >= INDEX_THRESHOLD_LIMIT) {
                    blacklist_key(key, conn, b_conn);
                    continue;
                }
                values = new t_value[val_length];
            }
            for (int i = 0; i < val_length && pos < INDEX_THRESHOLD_LIMIT; i++) {
                values[pos++] = (it->second)[i];
            }
            n_value->length = sizeof(t_value) * (pos);
            n_value->data = values;

            if (conn->put(key, n_value, INSERT_UPDATE) < 0) {
                cout << "throw insert exception" << endl;
            }

            delete[] values;
            delete n_value;
        }
        (it->second).clear();
        delete key;
        delete b_value;
    }
    conn->closeconnection();
    b_conn->closeconnection();
    delete conn;
    delete b_conn;
}

vector<t_value> Database::get(t_key k) {
    vector<t_key> keys(1, k);
    vector<vector<t_value> > values(1);
    thread_pool.enqueue([](Database * db, vector<t_key>* keys, size_t start, size_t end, vector<vector<t_value> >* values) {
        db->batch_get(keys, start, end, values);
        return 0;
    }, this, &keys, 0, 1, &values).get();
    return values[0];
}

vector<vector<t_value> > Database::get(vector<t_key>& keys) {
    size_t keys_size = keys.size();
    size_t batch_size = ceil((1.0 * keys_size) / num_threads);
    size_t start = 0, end = min(batch_size, keys_size);
    vector<vector<t_value> > values(keys_size);
    vector< future<int> > results;
    for (int i = 0; i < num_threads; i++) {
        results.emplace_back(
        thread_pool.enqueue([](Database * db, vector<t_key>* keys, size_t start, size_t end, vector<vector<t_value> >* values) {
            db->batch_get(keys, start, end, values);
            return 0;
        }, this, &keys, start, end, &values));
        start = start + batch_size;
        end = min(end + batch_size, keys_size);
    }
    for (auto && result : results)
        result.get();
    return values;
}

key_value_map Database::get(unordered_set<t_key>& keys) {
    ll keys_size = keys.size();
    ll batch_size = ceil((1.0 * keys_size) / num_threads);
    unordered_set<t_key>::iterator start, end;
    start = end = keys.begin();
    advance(end, batch_size);
    key_value_map values;
    vector< std::future<int> > results;
    for (int i = 0; i < num_threads; i++) {
        results.emplace_back(
        thread_pool.enqueue([](Database * db, unordered_set<t_key>* keys, unordered_set<t_key>::iterator start, unordered_set<t_key>::iterator end, key_value_map * values) {
            db->batch_get(keys, start, end, values);
            return 0;
        }, this, &keys, start, end, &values));
        keys_size = keys_size > batch_size ? keys_size - batch_size : 0 ;
        advance(start, keys_size == 0 ? 0 : batch_size);
        advance(end, min(keys_size, batch_size));
    }
    for (auto && result : results)
        result.get();
    return values;
}

void Database::batch_get(vector<t_key>* keys, size_t start, size_t end, vector<vector<t_value> >* values) {
    connection* conn = tbl->getconnection();
    connection* blacklisted_conn = b_tbl->getconnection();
    for (int i = start; i < end; i++) {
        FDT* key = new FDT(&(keys->at(i)), sizeof(t_key));
        FDT* b_value = blacklisted_conn->get(key);
        if (b_value && *(bool*) (b_value->data)) continue;
        FDT* value = conn->get(key);
        if (value) {
            size_t length = value->length / sizeof(t_value);
            (values->at(i)).assign((t_value*)value->data, (t_value*)value->data + length);
            value->free();
        }
        delete key;
        delete value;
    }
    blacklisted_conn->closeconnection();
    conn->closeconnection();
    delete blacklisted_conn;
    delete conn;
}

void Database::batch_get(unordered_set<t_key>* keys, unordered_set<t_key>::iterator start, unordered_set<t_key>::iterator end, key_value_map* data) {
    connection* conn = tbl->getconnection();
    connection* blacklisted_conn = b_tbl->getconnection();
    for (unordered_set<t_key>::iterator it = start; it != end; it++) {
        t_key m_key = *it;
        FDT* key = new FDT(&m_key, sizeof(t_key));
        FDT* b_value = blacklisted_conn->get(key);
        if (b_value && *(bool*) (b_value->data)) {
            b_value->free();
            key->free();
            continue;
        }
        FDT* value = conn->get(key);
        if (value) {
            size_t length = value->length / sizeof(t_value);
            data->at(m_key).assign((t_value*)value->data, (t_value*)value->data + length);
            //value->free();
        }
        delete key;
    }
    blacklisted_conn->closeconnection();
    conn->closeconnection();
    delete blacklisted_conn;
    delete conn;
}

void Database::remove(t_key k) {
    vector<t_key> keys(1, k);
    auto results = thread_pool.enqueue([](Database * db, vector<t_key>* keys, size_t start, size_t end) {
        db->batch_remove(keys, start, end);
        return 0;
    }, this, &keys, 0, 1).get();
}

void Database::remove(vector<t_key>& keys) {
    size_t data_size = keys.size();
    size_t batch_size = ceil((1.0 * data_size) / num_threads);
    size_t start = 0, end = min(batch_size, data_size);
    vector< future<int> > results;
    for (int i = 0; i < num_threads; i++) {
        results.emplace_back(
        thread_pool.enqueue([](Database * db, vector<t_key>* keys, size_t start, size_t end) {
            db->batch_remove(keys, start, end);
            return 0;
        }, this, &keys, start, end));
        start = start + batch_size;
        end = min(end + batch_size, data_size);
    }
    for (auto && result : results)
        result.get();
}

void Database::batch_remove(vector<t_key>* keys, size_t start, size_t end) {
    connection* conn = tbl->getconnection();
    connection* blacklisted_conn = b_tbl->getconnection();

    for (int i = start; i < end; i++) {
        FDT* key = new FDT(&(keys->at(i)), sizeof(t_key));
        blacklisted_conn->del(key);
        conn->del(key);
        delete key;
    }
    blacklisted_conn->closeconnection();
    conn->closeconnection();
    delete conn;
    delete blacklisted_conn;
}

Database::~Database() {
    if (db)
        close_database();
}
