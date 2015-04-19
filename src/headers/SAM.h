#ifndef SAM_H
#define SAM_H

#include "generic.h"
#include "util.h"
#include "constants.h"
#include "fq.h"

class SAM {
public:
    vector <fq> reads;

    SAM();
    ~SAM();
    void read_file(string);
    void write_sam(string);
};

#endif