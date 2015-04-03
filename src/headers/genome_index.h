#ifndef GENOME_INDEX
#define GENOME_INDEX

#include "database.h"

class GenomeIndex {
protected:
    int seed_len;
    Database db;
    Hash hash;

public:
    GenomeIndex(int seed_len) : seed_len(seed_len), hash(seed_len) {}
    ~GenomeIndex();
};
#endif