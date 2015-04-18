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
#include "headers/index_properties.h"

using namespace std;

void initialize_dash_dirs() {
    mkdir(cstr(PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(GENOME_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(DB_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void arihant_func(IndexProperties& ip) {
    GenomeCleaner g("chr.fa");
    g.clean();

    vector<string> chromosomes = g.get_chromosomes();
    for (int i = 1; i <= chromosomes.size(); i++) {
        ip.put_property(CHROMOSOME_PREFIX + pad(i, 3) , chromosomes[i - 1]);
    }
}

void shobhit_func(IndexProperties& ip, string f1, string f2, int seed_len = SEED_LEN) {
    GenomeIndexWriter giw(seed_len);
    giw.write_index(f1);
    giw.write_index(f2);
    giw.close_index();
    ip.put_property(SEED_LEN_PROP, to_string(seed_len));
    ip.write_file();

    // reading part
    IndexProperties ip_read;
    ip_read.read_file();
    int seed_len_read = stoi(ip_read.get_property(SEED_LEN_PROP));

    vector<string> reads;
    reads.push_back("ttttatgcggggatgcccccatgcaaaaatgcttttatgc");
    reads.push_back("ggggatgcccccatgcaaaaatgct");

    Aligner a(seed_len_read);
    a.align(reads);
}
int main(int argc, char *argv[]) {
    initialize_dash_dirs();

    IndexProperties ip;

    arihant_func(ip);
    shobhit_func(ip, argv[1], argv[2]);
    //arihant_func();
    //cout<<"Cleaned"<<endl;
    return 0;
}
