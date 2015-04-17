#include "headers/generic.h"
#include "headers/constants.h"
#include "headers/util.h"
#include "headers/database.h"
#include "headers/genome_cleaner.h"
#include "headers/file_buffer.h"
#include "headers/genome_index_writer.h"
#include "headers/genome_index_reader.h"
#include "headers/file_reader.h"
#include "headers/aligner.h"

using namespace std;

void initialize_dash_dirs() {
    mkdir(cstr(PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(GENOME_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(DB_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void arihant_func() {
}

void shobhit_func(string f1, string f2) {
    // GenomeIndexWriter g;
    // g.write_index(f1);
    // g.write_index(f2);
    vector<string> reads;
    reads.push_back("tacaattggccaattggccgtac");
    Aligner a;
    a.align(reads);
}
int main(int argc, char *argv[]) {
    initialize_dash_dirs();
    // arihant_func();
    shobhit_func(argv[1], argv[2]);
    return 0;
}