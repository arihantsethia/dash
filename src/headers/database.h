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
    table *tbl;
    table_env db_properties;
    ThreadPool thread_pool;

    void batch_get(vector<t_key>*, size_t, size_t, vector<vector<t_value> >*);
    void batch_put(vector<key_value>*, size_t, size_t);
    void batch_remove(vector<t_key>*, size_t, size_t);
public:
    enum TransactionMode { DISABLED, ENABLED };

    Database(string dbPath = DB_PATH, int num_threads = NUM_THREADS, string configPath = DB_CONFIG, string logPath = DB_LOG, TransactionMode transMode = ENABLED);

    void use_table(int);
    void close_table();
    void close_database();

    vector<t_value> get(t_key);
    vector<vector<t_value> > get(vector<t_key>&);
    void put(key_value);
    void put(vector<key_value>&);
    void remove(t_key);
    void remove(vector<t_key>&);

    ~Database();
};

#endif
