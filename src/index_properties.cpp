#include "headers/index_properties.h"

IndexProperties::IndexProperties(string index_prop_file) : index_prop_file(index_prop_file) {
    read_file();
}

void IndexProperties::read_file() {
    ifstream fin;
    try {
        fin.open(index_prop_file);
        string line;
        while (getline(fin, line))
        {
            std::vector<string> tokens = tokenize(line, "\t");
            assert(tokens.size() >= 2); // TODO: throw an exception
            prop_map[tokens[0]] = tokens[1];
        }
        fin.close();
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file reading" << index_prop_file << endl;
        if (fin.is_open()) {
            fin.close();
        }
    }
}

void IndexProperties::write_file() {
    ofstream fout;
    try {
        fout.open(index_prop_file, ios_base::trunc);
        for (auto& e : prop_map) {
            fout << e.first << "\t" << e.second << endl;
        }
        fout.close();
    }
    catch (ofstream::failure e) {
        cerr << "Exception in file writing" << index_prop_file << endl;
        if (fout.is_open()) {
            fout.close();
        }
    }
}

string IndexProperties::get_property(string key) {
    if (prop_map.find(key) != prop_map.end()) {
        return prop_map[key];
    }
    else {
        assert(false);  //TODO : throw an exception
    }
}

void IndexProperties::put_property(string key, string val) {
    prop_map[key] = val;
}
