#ifndef DATABASE_H
#define DATABASE_H

#include "generic.h"
#include "util.h"
#include <bangdb/tableenv.h>
#include <bangdb/database.h>

using namespace std;
using namespace bangdb;

class Database{
private:
    static const string DB_NAME;
    static const string TABLE_PREFIX;
    static const string CONFIG;
    static const int NUM_THREADS;

    string dbPath;
    string logPath;

    database *db;
    table *tbl;
    vector<connection*> db_conns;
    table_env db_properties;

    void open_connections();
    void close_connections();
    vector<value> get(int, vector<key>&);
    void put(int, vector<key_value>&);
    void remove(int, vector<key>&);

public:
    enum TransactionMode { DISABLED, ENABLED };
    
    Database(string, TransactionMode transMode = ENABLED);
    Database(string, string, TransactionMode transMode = ENABLED);

    void use_table(int);
    void close_table();
    void close_database();

    vector<value> get(key);
    vector<value> get(vector<key>&);
    void put(key_value);
    void put(vector<key_value>&);
    void remove(key);
    void remove(vector<key>&);
    //bool update(int, vector<int>&);

    void setNumberOfThreads(int);

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
