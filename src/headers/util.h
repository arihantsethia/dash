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

#endif