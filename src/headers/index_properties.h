#ifndef INDEX_PROPERTIES
#define INDEX_PROPERTIES
#include "util.h"

class IndexProperties {
private:
    std::map<string, string> prop_map;
    string index_prop_file;
public:
    IndexProperties(string index_prop_file);
    void read_file();
    void write_file();
    string get_property(string key);
    void put_property(string key, string val);
    ~IndexProperties();

};
#endif