#ifndef ALIGNER
#define ALIGNER

#include "constants.h"
#include "index_properties.h"
#include "genome_index_reader.h"
#include "read_aligner.h"
using namespace std;

class Aligner {
private:
    int seed_len;
    
public:
    Aligner(int seed_len = SEED_LEN);
    void align(vector<string>& reads);
};
#endif