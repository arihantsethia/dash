#ifndef argument_parser
#define argument_parser value

#include "constants.h"
using namespace std;

class ArgumentParser {
public:
    string genome, in, out;
    int seed_len, threads, edit_dist, conf;
    bool index, align, distributed;
    
    ArgumentParser();
    int parse_args(int argc, char* argv[]);
    void print_usage();
};
#endif