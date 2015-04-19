#ifndef ALIGNER
#define ALIGNER

#include "constants.h"
#include "index_properties.h"
#include "genome_index_reader.h"
#include "read_aligner.h"
#include "fq.h"

class Aligner {
private:
    int seed_len, num_chromo, threads, ed_threshold, conf;
    IndexProperties ip;

public:
    Aligner(int num_chromo, IndexProperties ip, int seed_len = SEED_LEN, int threads = NUM_THREADS, int ed_threshold = MAX_EDIT_DIST, int conf = CONFIDENCE_THRESHOLD);
    void align(vector<fq>& reads);
};
#endif