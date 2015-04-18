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
    GenomeIndexReader(int seed_len = SEED_LEN, int threads = NUM_THREADS, int num_chromo = NUM_CHROMOSOMES);
    vector<vector<unordered_set<t_value> > > get_positions(vector<string>& reads);
};
#endif