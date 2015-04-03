#include "headers/database.h"

Database::Database(string dbPath, int num_threads, string configPath, string logPath, TransactionMode transMode) : thread_pool(num_threads), num_threads(num_threads) {
    if (transMode == ENABLED) {
        db = new database(cstr(DB_NAME), cstr(configPath), DB_OPTIMISTIC_TRANSACTION, cstr(dbPath), cstr(logPath));
    } else {
        db = new database(cstr(DB_NAME), cstr(configPath), DB_TRANSACTION_NONE, cstr(dbPath), cstr(logPath));
    }
    tbl = NULL;
}

void Database::close_database() {
    if (db) {
        db->closedatabase();
    }
}

void Database::use_table(int id) {
    close_table();
    if ((tbl = db->gettable(cstr(TABLE_PREFIX + istr(id)), OPENCREATE)) == NULL) {
        cout << "throw exception table" << endl;
    }
}

void Database::close_table() {
    if (tbl) {
        tbl->closetable();
    }
}

void Database::put(key_value data) {
    vector<key_value> v(1, data);
    thread_pool.enqueue([](Database* db, vector<key_value>* data, size_t start, size_t end) {
            db->batch_put(data, start, end);
            return 0;
        }, this, &v, 0, 1).get();
}

void Database::put(vector<key_value>& data) {
    size_t data_size = data.size();
    size_t batch_size = ceil((1.0 * data_size) / num_threads);
    size_t start = 0, end = min(batch_size, data_size);
    vector< std::future<int> > results;
    for (int i = 0; i < num_threads; i++) {
        results.emplace_back(
        thread_pool.enqueue([](Database* db, vector<key_value>* data, size_t start, size_t end) {
            db->batch_put(data, start, end);
            return 0;
        }, this, &data, start, end));
        start = start + batch_size;
        end = min(end + batch_size, data_size);
    }
    for (auto&& result : results)
        result.get();
}

void Database::batch_put(vector<key_value>* data, size_t start, size_t end) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for(int i=start; i<end; i++) {
            FDT* key = new FDT(&(data->at(i)).first, sizeof(t_key));
            FDT* o_value = conn->get(key);
            FDT* n_value = new FDT();
            t_value* values;
            int pos = 0;
            if (o_value != NULL) {
                pos = o_value->length / sizeof(t_value);
                values = new t_value[pos + 1];
                copy(values, (t_value*)o_value->data, pos);
                o_value->free();
            } else {
                values = new t_value[1];
            }
            values[pos] = (data->at(i)).second;
            n_value->length = sizeof(t_value) * (pos + 1);
            n_value->data = values;
            if (conn->put(key, n_value, INSERT_UPDATE) < 0) {
                cout << "throw insert exception" << endl;
            }
            delete key;
            delete n_value;
            delete o_value;
            delete[] values;
        }
        conn->closeconnection();
        delete conn;
    } else {
        cout<<"Couldn't establish connection"<<endl;
    }
}

vector<t_value> Database::get(t_key k) {
    vector<t_key> keys(1, k);
    vector<vector<t_value> > values(1);
    thread_pool.enqueue([](Database* db, vector<t_key>* keys, size_t start, size_t end, vector<vector<t_value> >* values) {
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
        thread_pool.enqueue([](Database* db, vector<t_key>* keys, size_t start, size_t end, vector<vector<t_value> >* values) {
            db->batch_get(keys, start, end, values);
            return 0;
        }, this, &keys, start, end, &values));
        start = start + batch_size;
        end = min(end + batch_size, keys_size);
    }
    for(auto&& result : results)
        result.get();
    return values;
}

void Database::batch_get(vector<t_key>* keys, size_t start, size_t end, vector<vector<t_value> >* values) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for (int i=start; i<end; i++){
            FDT* key = new FDT(&(keys->at(i)), sizeof(t_key));
            FDT* value = conn->get(key);
            if (value) {
                size_t length = value->length / sizeof(t_value);
                (values->at(i)).assign((t_value*)value->data, (t_value*)value->data + length);
                value->free();
            }
            delete key;
            delete value;
        }
        conn->closeconnection();
        delete conn;
    } else {
        cout << "throw connection exception" << endl;
    }
}

void Database::remove(t_key k) {
    vector<t_key> keys(1, k);
    auto results = thread_pool.enqueue([](Database* db, vector<t_key>* keys, size_t start, size_t end) {
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
        thread_pool.enqueue([](Database* db, vector<t_key>* keys, size_t start, size_t end) {
            db->batch_remove(keys, start, end);
            return 0;
        }, this, &keys, start, end));
        start = start + batch_size;
        end = min(end + batch_size, data_size);
    }
    for (auto&& result : results)
        result.get();
}

void Database::batch_remove(vector<t_key>* keys, size_t start, size_t end) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for (int i=start; i<end;i++) {
            FDT* key = new FDT(&(keys->at(i)), sizeof(t_key));
            if (conn->del(key) < 0) {
                cout << "throw delete exception" << endl;
            }
            delete key;
        }
        conn->closeconnection();
    } else {
        cout << "throw connection exception" << endl;
    }
    delete conn;
}

Database::~Database() {
    thread_pool.join();
    close_table();
    close_database();
    delete tbl;
    delete db;
}
