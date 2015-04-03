#ifndef GENOME_CLEANER
#define GENOME_CLEANER

#include "generic.h"

class GeneomeCleaner{
	string genome_file;
	string genome_dir;
public:
	GeneomeCleaner(string genome_file, string genome_dir);
	~GeneomeCleaner();
};

#endif