#include "headers/generic.h"
#include "headers/constants.h"
#include "headers/util.h"
#include "headers/database.h"
#include "headers/genome_cleaner.h"
#include "headers/file_buffer.h"
#include "headers/genome_index_writer.h"

using namespace std;

void initialize_dash_dirs() {
    mkdir(cstr(PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(GENOME_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(DB_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void arihant_func() {
    GenomeCleaner g("chr1.fa");
    g.clean();
}

void shobhit_func(string filename) {
    GenomeIndexWriter g;
    g.write_index(filename);
}
int main(int argc, char *argv[]) {
    initialize_dash_dirs();
    //arihant_func();
    //cout<<"Cleaned"<<endl;
    shobhit_func(GENOME_PATH+PATH_SEPERATOR+"chromosome_001");
    return 0;
}
