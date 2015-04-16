#ifndef DATABASE_H
#define DATABASE_H

#include "generic.h"
#include "util.h"
#include "constants.h"
#include "thread_pool.h"
#include <bangdb/tableenv.h>
#include <bangdb/database.h>

using namespace bangdb;

class Database {
private:
    int num_threads;

    database *db;
    table* tbl;
    table* b_tbl;
    ThreadPool thread_pool;

    void batch_get(vector<t_key>*, size_t, size_t, vector<vector<t_value> >*);
    void batch_get(unordered_set<t_key>*, unordered_set<t_key>::iterator, unordered_set<t_key>::iterator, key_value_map*);
    void batch_put(key_value_map*, key_value_map::iterator, key_value_map::iterator);
    void batch_remove(vector<t_key>*, size_t, size_t);
    void blacklist_key(FDT*, connection*, connection*);

    table_env* get_table_properties(bool);

public:
    enum TransactionMode { DISABLED, ENABLED };

    Database(string dbPath = DB_PATH, int num_threads = NUM_THREADS, string configPath = DB_CONFIG, string logPath = DB_LOG, TransactionMode transMode = DISABLED);

    void use_table(int);
    void close_table();
    void close_database();

    vector<t_value> get(t_key);
    vector<vector<t_value> > get(vector<t_key>&);
    key_value_map get(unordered_set<t_key>&);
    void put(key_value);
    void put(key_value_map&);
    void remove(t_key);
    void remove(vector<t_key>&);

    ~Database();
};

#endif
