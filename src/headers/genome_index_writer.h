#ifndef GENOME_INDEX_WRITER
#define GENOME_INDEX_WRITER 

#include "util.h"
#include "genome_index.h"
#include "file_buffer.h"
// #include "hash.h"

class GenomeIndexWriter : private GenomeIndex
{
private:
public:
    GenomeIndexWriter(int seed_len);
    void write_index(string filename);
    ~GenomeIndexWriter();
    
};
#endif