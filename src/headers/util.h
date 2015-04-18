#ifndef UTIL_H
#define UTIL_H

#include "generic.h"

static unordered_map<char, int> dna = {{'a', 0}, {'c', 1}, {'g', 2}, {'t', 3}};

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

static inline char get_base(char c) {
    if (dna.find(tolower(c)) != dna.end())
        return tolower(c);
    return 0;
}

static inline bool is_valid_dna(string s) {
    for (int i = 0; i < s.length(); i++)
        if (dna.find(tolower(s[i])) == dna.end())
            return false;
    return true;
}

static inline string pad(int num, int len) {
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
static vector<string> tokenize(string s, string sep) {
    // Skip delimiters at beginning.
    string::size_type lastPos = s.find_first_not_of(sep, 0);
    // Find first "non-delimiter", which will be between lastPos and pos
    string::size_type pos = s.find_first_of(sep, lastPos);
    vector<string> tokens;
    while (pos != string::npos || lastPos != string::npos) {
        tokens.push_back(s.substr(lastPos, (pos - lastPos)));
        // Skip delimiters
        lastPos = s.find_first_not_of(sep, pos);
        // Find "non-delimiter", which will be between lastPos and pos
        pos = s.find_first_of(sep, lastPos);
    }
    return tokens;
}

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v") {
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v") {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v") {
    return ltrim(rtrim(s, t), t);
}

#endif