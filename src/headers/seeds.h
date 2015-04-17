#ifndef SEEDS
#define SEEDS value

#include "constants.h"
#include "generic.h"
#include "hash.h"
#include "constants.h"
using namespace std;

class Seeds {
private:
    int seed_len;
    Hash hash;
public:
    Seeds(int seed_len = SEED_LEN): seed_len(seed_len), hash(seed_len) {}
    vector<t_key> get_seeds(string rd);
};

#endif