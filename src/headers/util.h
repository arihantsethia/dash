#ifndef UTIL_H
#define UTIL_H

#include "generic.h"

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