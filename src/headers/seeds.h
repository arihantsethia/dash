#ifndef SEEDS
#define SEEDS value

#include "constants.h"
using namespace std;

class Seeds {
private:
    int seed_len;
public:
    Seeds(int seed_len = SEED_LEN): seed_len(seed_len) {}
    vector<string> get_seeds(string rd);
};

#endif