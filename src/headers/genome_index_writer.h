#ifndef GENOME_INDEX_WRITER
#define GENOME_INDEX_WRITER

#include "util.h"
#include "genome_index.h"
#include "file_buffer.h"

class GenomeIndexWriter : private GenomeIndex {
public:
    GenomeIndexWriter(int seed_len = SEED_LEN, int threads = NUM_THREADS);
    void write_index(string filename, t_value pos);
    void close_index();
};

#endif