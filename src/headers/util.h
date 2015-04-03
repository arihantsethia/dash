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

static inline char get_base(char c){
    switch(tolower(c)){
        case 'a':
        case 'c':
        case 'g':
        case 't':
            return tolower(c);
        default:
            return 0;
    }
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

#endif