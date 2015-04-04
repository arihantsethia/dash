#ifndef HASH
#define HASH

#include "generic.h"
#include "util.h"

class Hash {
private:
    static const unordered_map<char, int> char_to_bit;
    static const unordered_map<int, char> bit_to_char;
    int length;
public:
    Hash(int);
    ~Hash();
    void set_length(int);
    t_value get_hash_value(string);
    string get_value(t_value);
};
#endif