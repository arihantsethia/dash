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
    for (auto && result : results)
        result.get();
}

void Database::batch_put(vector<key_value>& data) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for (key_value elem : data) {
            FDT* value = conn->get_data(elem.first);
            if (value == NULL) {
                value = new FDT();
                value->data = new vector<t_value>();
            }
            ((vector<t_value>*)(value->data))->push_back(elem.second);
            value->length = sizeof(std::vector<t_value>) + (sizeof(t_value) * ((vector<t_value>*)(value->data))->size());
            if (conn->put(elem.first, value, INSERT_UPDATE) < 0) {
                cout << "throw insert exception" << endl;
            }
        }
        conn->closeconnection();
    } else {
        cout << "throw connection exception" << endl;
    }
}

vector<t_value> Database::get(t_key k) {
    vector<t_key> keys(1, k);
    vector<vector<t_value> > results = batch_get(keys);
    vector<t_value> result;
    if (results.size()) {
        result = results[0];
    }
    return result;
}

vector<t_value> Database::get(vector<t_key>& keys) {
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
    for (auto && result : results)
        result.get();
}

vector<vector<t_value> > Database::batch_get(vector<t_key>& keys) {
    vector<vector<t_value> > res;
    return res;
}

void Database::remove(t_key k) {
    vector<t_key> keys(1, k);
    batch_remove(keys);
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
    for (auto && result : results)
        result.get();
}

void Database::batch_remove(vector<t_key>& keys) {
    connection* conn = tbl->getconnection();
    if (conn) {
        for (t_key key : keys) {
            if (conn->del(key) < 0) {
                cout << "throw delete exception" << endl;
            }
        }
        conn->closeconnection();
    } else {
        cout << "throw connection exception" << endl;
    }
}

Database::~Database() {
    thread_pool.join();
    close_table();
    close_database();
}
