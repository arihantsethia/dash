#include "headers/hash.h"

const unordered_map<char, int> Hash::char_to_bit = {{'a', 0}, {'c', 1}, {'g', 2}, {'t', 3}};
const unordered_map<int, char> Hash::bit_to_char = {{0, 'a'}, {1, 'c'}, {2, 'g'}, {3, 't'}};

Hash::Hash(int length): length(length) {}

Hash::~Hash() {}

void Hash::set_length(int len) {
    length = len;
}

t_value Hash::get_hash_value(string s) {
    if (s.length() != length || !is_valid_dna(s)) {
        cout << "Throw some error" << endl;
    }
    t_value hash_val = 2;
    for (int i = 0; i < length; i++) {
        hash_val = (hash_val << 2) | char_to_bit.at(s[i]);
    }
    return hash_val;
}

string Hash::get_value(t_value v) {
    string s(length, ' ');
    for (int i = length - 1; i >= 0; i--) {
        s[i] = bit_to_char.at(v & 3);
    }
    return s;
}