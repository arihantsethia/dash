#include "headers/seeds.h"

// TODO: should I hash here? If yes, should I use two vectors?
vector<string> Seeds::get_seeds(string rd) {
    int n = (rd.size() + seed_len - 1) / seed_len;
    vector<string> seeds(n);
    int pos = 0;

    string temp(seed_len, ' ');
    //insert all seeds into vector except last one
    for (int i = 0; i < n - 1; i = i + seed_len) {
        for (int j = 0; j < seed_len; ++j) {
            temp[j] = rd[i * seed_len + j];
        }
        seeds[pos] = temp;
        pos++;
    }

    // insert last seed into vector explicitly
    // done to handle irregular division cases and perfect division cases
    // without unnecessary condition checks and code duplication

    for (int i = 0; i < seed_len ; ++i) {
        temp[seed_len - i - 1] = rd[rd.size() - i - 1];
    }
    seeds[pos] = temp;
    return seeds;
}