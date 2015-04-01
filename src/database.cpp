#include "headers/database.h"

const int Database::NUM_THREADS = 4;
const string Database::DB_NAME = "dash_db";
const string Database::TABLE_PREFIX = "index_chromosome_";
const string Database::CONFIG = "db.config";


// TODO : ERROR HANDLING + THROW EXCEPTION

Database::Database(string dbPath, TransactionMode transMode) : dbPath(dbPath) {
    if (transMode == ENABLED) {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_OPTIMISTIC_TRANSACTION, cstr(dbPath));
    } else {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_TRANSACTION_NONE, cstr(dbPath));
    }
    setNumberOfThreads(NUM_THREADS);
}

Database::Database(string dbPath, string logPath, TransactionMode transMode) : dbPath(dbPath), logPath(logPath) {
    if (transMode == ENABLED) {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_OPTIMISTIC_TRANSACTION, cstr(dbPath), cstr(logPath));
    } else {
        db = new database(cstr(DB_NAME), cstr(CONFIG), DB_TRANSACTION_NONE, cstr(dbPath), cstr(logPath));
    }
    setNumberOfThreads(NUM_THREADS);
}

void Database::setNumberOfThreads(int num_threads) {
    db_conns.resize(num_threads);
    open_connections();
}

void Database::close_database(){
    if(db)
        db->closedatabase();
}

void Database::use_table(int id){
    tbl = db->gettable(cstr(TABLE_PREFIX + istr(id)), OPENCREATE);
    open_connections();
}

void Database::close_table(){
    if(tbl)
        tbl->closetable();
}

void Database::open_connections(){
    for(int i=0;i<db_conns.size();i++){
        db_conns[i] = tbl->getconnection();
    }
}

void Database::close_connections(){
    for(int i=0;i<db_conns.size();i++){
        if(db_conns[i]==NULL){
            db_conns[i]->closeconnection();
        }
    }
}

void Database::put(key_value data){
    vector<key_value> v(1,data);
    put(0, v);
}

void Database::put(vector<key_value>& data){
    /*
    pthread_t thrds[db_conns.size()];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int data_size = data.size();
    int batch_size = ceil(data_size/db_conns.size());
    int start = 0;
    int end = min(batch_size, data_size);

    for(int i=0;i<db_conns.size();i++){
        put_arg args;
        args.conn_id = i;
        args.data.assign(data.begin()+start, data.begin()+end);
        //TODO : ERROR CHECKING IN CREATING THREADS
        pthread_create(&thrds[i], &attr,thread_put_args, (void*)&args);
        start = start + batch_size;
        end = min(end+batch_size, data_size);
    }
    void *status;
    for(int i = 0; i<db_conns.size(); i++) {
        //TODO : ERROR CHECKING IN THREAD COMPLETION
        pthread_join(thrds[i], &status);
    }*/
}

void Database::put(int conn_id, vector<key_value>& data){
    if(db_conns[conn_id]){
        for(int i=0;i<data.size();i++){
            FDT* value = db_conns[conn_id]->get_data(data[i].first);
            vector<ull> val;
            if(!value){
                value->data = &val; 
            }
            val = *(vector<ull>*)(value->data);
            val.push_back(data[i].second);
            value->length = sizeof(std::vector<ull>) + (sizeof(ull) * val.size());
            db_conns[conn_id]->put(data[i].first, value,INSERT_UPDATE);
        }
    }else{
        cout<<"Shiiiiiiiiiiiiiit!!!!!";
        //THROW SOME EXCEPTION
    }
}

vector<value> Database::get(key k){
    vector<key> keys(1,k);
    get(0, keys);
}

vector<value> Database::get(vector<key>& keys){
    //SOME SHIT
}

vector<value> Database::get(int conn_id, vector<key>& keys){
    //SOME SHIT
}

void Database::remove(key k){
    vector<key> keys(1,k);
    remove(0, keys);
}

void Database::remove(vector<key>& keys){
    //SOME SHIT
}

void Database::remove(int conn_id, vector<key>& keys){
    //SOME SHIT
}

Database::~Database() {
    close_connections();
    close_table();
    close_table();
}
