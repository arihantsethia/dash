#include "headers/database.h"

const int Database::NUM_THREADS = 4;
const string Database::DB_NAME = "dash_db";
const string Database::TABLE_PREFIX = "index_chromosome_";
const string Database::CONFIG = "config";

Database::Database(string dbPath, TransactionMode transMode, int num_threads) : dbPath(dbPath), thread_pool(num_threads), num_threads(num_threads)  {
    if (transMode == ENABLED) {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_OPTIMISTIC_TRANSACTION, cstr(dbPath));
    } else {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_TRANSACTION_NONE, cstr(dbPath));
    }
}

Database::Database(string dbPath, string logPath, TransactionMode transMode, int num_threads) : dbPath(dbPath), logPath(logPath), thread_pool(num_threads), num_threads(num_threads) {
    if (transMode == ENABLED) {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_OPTIMISTIC_TRANSACTION, cstr(dbPath), cstr(logPath));
    } else {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_TRANSACTION_NONE, cstr(dbPath), cstr(logPath));
    }
}

void Database::close_database() {
    if (db) {
        db->closedatabase();
    }
}

void Database::use_table(int id) {
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
    batch_put(v);
}

void Database::put(vector<key_value>& data) {
    size_t data_size = data.size();
    size_t batch_size = ceil((1.0 * data_size) / num_threads);
    size_t start = 0, end = min(batch_size, data_size);
    vector< std::future<int> > results;
    vector<key_value> batch_data(batch_size);
    for (int i = 0; i < num_threads; i++) {
        batch_data.assign(data.begin() + start, data.begin() + end);
        results.emplace_back(
        thread_pool.enqueue([](Database * db, vector<key_value> data) {
            db->batch_put(data);
            return 0;
        }, this, batch_data));
        start = start + batch_size;
        end = min(end + batch_size, data_size);
    }
    for (auto&& result : results)
        result.get();
}

void Database::batch_put(vector<key_value>& data) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for (auto elem : data) {
            FDT* key = new FDT(&elem.first, sizeof(t_key));
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
            values[pos] = elem.second;
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
        cout << "throw connection exception" << endl;
    }
}

vector<t_value> Database::get(t_key k) {
    vector<t_key> keys(1, k);
    auto results = thread_pool.enqueue([](Database * db, vector<t_key> keys) {
        return db->batch_get(keys);
    }, this, keys).get();
    vector<t_value> result;
    if (results.size()) {
        result = results[0];
    }
    return result;
}

vector<vector<t_value> > Database::get(vector<t_key>& keys) {
    size_t keys_size = keys.size();
    size_t batch_size = ceil((1.0 * keys_size) / num_threads);
    size_t start = 0, end = min(batch_size, keys_size);
    vector<vector<t_value> > result(keys_size);
    vector< std::future< vector<vector<t_value> > > > results;
    vector<t_key> batch_keys(batch_size);
    for (int i = 0; i < num_threads; i++) {
        batch_keys.assign(keys.begin() + start, keys.begin() + end);
        results.emplace_back(
        thread_pool.enqueue([](Database * db, vector<t_key> data) {
            return db->batch_get(data);
        }, this, batch_keys));
        start = start + batch_size;
        end = min(end + batch_size, keys_size);
    }
    for (auto&& res : results) {
        for (auto elem : res.get()) {
            result.push_back(elem);
        }
    }
    return result;
}

vector<vector<t_value> > Database::batch_get(vector<t_key>& keys) {
    connection* conn = tbl->getconnection();
    vector<vector<t_value> > results;
    if (conn) {
        for (t_key key : keys) {
            FDT* k = new FDT(&key, sizeof(t_key));
            FDT* v = conn->get(k);
            vector <t_value> result;
            if (v) {
                size_t length = v->length / sizeof(t_value);
                result.assign((t_value*)v->data, (t_value*)v->data + length);
                v->free();
            }
            results.push_back(result);
            delete k;
            delete v;
        }
        conn->closeconnection();
        delete conn;
    } else {
        cout << "throw connection exception" << endl;
    }
    return results;
}

void Database::remove(t_key k) {
    vector<t_key> keys(1, k);
    auto results = thread_pool.enqueue([](Database * db, vector<t_key> keys) {
        db->batch_remove(keys);
        return 0;
    }, this, keys).get();
}

void Database::remove(vector<t_key>& keys) {
    size_t data_size = keys.size();
    size_t batch_size = ceil((1.0 * data_size) / num_threads);
    size_t start = 0, end = min(batch_size, data_size);
    vector< std::future<int> > results;
    vector<t_key> batch_data(batch_size);
    for (int i = 0; i < num_threads; i++) {
        batch_data.assign(keys.begin() + start, keys.begin() + end);
        results.emplace_back(
        thread_pool.enqueue([](Database * db, vector<t_key> data) {
            db->batch_remove(data);
            return 0;
        }, this, batch_data));
        start = start + batch_size;
        end = min(end + batch_size, data_size);
    }
    for (auto&& result : results)
        result.get();
}

void Database::batch_remove(vector<t_key>& keys) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for (auto key : keys) {
            FDT* k = new FDT(&key, sizeof(t_key));
            if (conn->del(k) < 0) {
                cout << "throw delete exception" << endl;
            }
            delete k;
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
