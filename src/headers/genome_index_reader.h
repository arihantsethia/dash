#ifndef GENOME_INDEX_READER
#define GENOME_INDEX_READER value

#include "genome_index.h"
#include "constants.h"
#include "database.h"
#include "seeds.h"
using namespace std;

class GenomeIndexReader : private GenomeIndex {
private:
    int num_chromo;
public:
    GenomeIndexReader(int num_chromo, int seed_len = SEED_LEN, int threads = NUM_THREADS);
    vector<vector<unordered_set<t_value> > > get_positions(vector<string>& reads);
};
#endif