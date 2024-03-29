#ifndef GENOME_INDEX
#define GENOME_INDEX

#include "database.h"
#include "hash.h"

class GenomeIndex {
protected:
    int seed_len;
    Database db;
    Hash hash;

public:
    GenomeIndex(int seed_len = SEED_LEN, int threads = NUM_THREADS) : seed_len(seed_len), hash(seed_len), db(threads) {}
};

#endif