#include "headers/generic.h"
#include "headers/constants.h"
#include "headers/util.h"
#include "headers/database.h"

using namespace std;

void initialize_dash_dirs(){
    mkdir(cstr(PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(GENOME_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(DB_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void arihant_func(){
    /*
    Database a;
    a.use_table(1);
    vector<key_value> s;
    vector<t_value> k;
    for(int i=1;i<250;i++){
        s.push_back(make_pair(i,2*i));
        k.push_back(i);
    }
    a.put(s);
    cout<<"write finished1"<<endl;
    //a.put(s);
    cout<<"write finished"<<endl;
    cout<<"get started"<<endl;
    vector<vector<t_value> >&& res = a.get(k);
    cout<<res.size()<<endl;*/
}

int main(){
    initialize_dash_dirs();
    arihant_func();
    return 0;
}