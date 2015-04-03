#ifndef DATABASE_H
#define DATABASE_H

#include "generic.h"
#include "util.h"
#include "thread_pool.h"
#include <bangdb/tableenv.h>
#include <bangdb/database.h>

using namespace std;
using namespace bangdb;

class Database {
private:
    static const string DB_NAME;
    static const string TABLE_PREFIX;
    static const string CONFIG;
    static const int NUM_THREADS;

    string dbPath;
    string logPath;

    int num_threads;

    database *db;
    table *tbl;
    table_env db_properties;
    ThreadPool thread_pool;

    vector<vector<t_value> > batch_get(vector<t_key>&);
    void batch_put(vector<key_value>&);
    void batch_remove(vector<t_key>&);
public:
    enum TransactionMode { DISABLED, ENABLED };

    Database(string, TransactionMode transMode = ENABLED, int num_threads = NUM_THREADS);
    Database(string, string, TransactionMode transMode = ENABLED, int num_threads = NUM_THREADS);

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

/*
class DatabaseException: public exception
{
private:
    string message;
public:
    virtual const char* what() const throw(){
        return "My exception happened";
    }
};
*/
#endif
