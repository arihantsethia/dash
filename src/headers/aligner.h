#ifndef ALIGNER
#define ALIGNER

#include "constants.h"
#include "index_properties.h"
#include "genome_index_reader.h"
#include "read_aligner.h"
using namespace std;

class Aligner {
private:
    int seed_len, num_chromo, threads, ed_threshold, conf;

public:
    Aligner(int seed_len = SEED_LEN, int threads = NUM_THREADS, int ed_threshold = MAX_EDIT_DIST, int conf = CONFIDENCE_THRESHOLD, int num_chromo = NUM_CHROMOSOMES );
    void align(vector<string>& reads);
};
#endif