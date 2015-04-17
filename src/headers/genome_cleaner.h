#ifndef GENOME_CLEANER
#define GENOME_CLEANER

#include "generic.h"
#include "util.h"
#include "constants.h"

class GenomeCleaner {
private:
    // static const string CHROMOSOME_PREFIX;
    static const int BUFFER_SIZE;


    string genome_file;
    string genome_dir;

    string chromosome_file_path(int id);
public:
    GenomeCleaner(string genome_file, string genome_dir = GENOME_PATH);

    void clean();
};

#endif