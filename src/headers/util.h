#ifndef UTIL_H
#define UTIL_H

#include "generic.h"

static unordered_map<char,int> dna = {{'a', 0}, {'c', 1}, {'g', 2}, {'t', 3}};

static inline const char* cstr(const string str) {
    return str.c_str();
}

static inline string istr(int i) {
    return to_string(i);
}

static inline const string get_path_seperator() {
    #ifdef _WIN32
        return "\\";
    #else
        return "/";
    #endif
}

static inline char get_base(char c){
    if(dna.find(tolower(c))!=dna.end())
        return tolower(c);
    return 0;
}

static inline bool is_valid_dna(string s){
    for(int i=0;i<s.length();i++)
        if(dna.find(tolower(s[i]))==dna.end())
            return false;
    return true;
}

static inline string pad(int num, int len){
    string s = istr(num);
    s.insert(s.begin(), len - s.size(), '0');
    return s;
}

template<typename T>
static inline void copy(T* &v1, T* v2, int size) {
    for (int i = 0; i < size; i++) {
        v1[i] = v2[i];
    }
}

/*
    Function to tokenize a given string on the seperator and
    return a vector of the tokenized strings.
*/
std::vector<std::string> tokenize(std::string s, std::string sep) {
    // Skip delimiters at beginning.
    std::string::size_type lastPos = s.find_first_not_of(sep, 0);
    // Find first "non-delimiter", which will be between lastPos and pos
    std::string::size_type pos = s.find_first_of(sep, lastPos);
    std::vector<std::string> tokens;
    while (pos != std::string::npos || lastPos != std::string::npos) {
        tokens.push_back(s.substr(lastPos, (pos - lastPos)));
        // Skip delimiters
        lastPos = s.find_first_not_of(sep, pos);
        // Find "non-delimiter", which will be between lastPos and pos
        pos = s.find_first_of(sep, lastPos);
    }
    return tokens;
}

#endif