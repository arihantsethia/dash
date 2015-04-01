#ifndef UTIL_H
#define UTIL_H

#include "generic.h"
using namespace std;

static inline const char* cstr(const string str){
	return str.c_str();
}

static inline string istr(int i){
	return to_string(i);
}


#endif