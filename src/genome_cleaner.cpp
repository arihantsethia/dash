#include "headers/genome_cleaner.h"

const int GenomeCleaner::BUFFER_SIZE = 100 * 1024 * 1024;

GenomeCleaner::GenomeCleaner(string genome_file, string genome_dir): genome_dir(genome_dir), genome_file(genome_file) {
}
void GenomeCleaner::clean() {
    ifstream i_file (genome_file);
    ofstream o_file;
    bool skip = false;
    int chromosome_counter = 1;
    char* rbuffer = new char[BUFFER_SIZE + 1];
    char* wbuffer = new char[BUFFER_SIZE + 1];
    memset(wbuffer, 0, BUFFER_SIZE + 1);
    size_t wlength = 0, rlength = 0;
    string chromosome_name;
    while (i_file) {
        memset(rbuffer, 0, BUFFER_SIZE + 1);
        i_file.read(rbuffer, BUFFER_SIZE);
        rlength = 0;
        while (rbuffer[rlength]) {
            char curr_chr = rbuffer[rlength];
            if (curr_chr == '>') {
                skip = true;
                chromosome_name = "";
                if (o_file.is_open()) {
                    o_file.write(wbuffer, wlength);
                    o_file.close();
                }
                memset(wbuffer, 0, BUFFER_SIZE + 1);
                wlength = 0;
                o_file.open(chromosome_file_path(chromosome_counter++));
            }
            else if (curr_chr == '\n') {
                if (skip) {
                    chromosomes.push_back(trim(chromosome_name));
                }
                skip = false;
            }

            if (!skip && o_file.is_open()) {
                if (get_base(curr_chr)) {
                    wbuffer[wlength++] = get_base(curr_chr);
                    if (wlength == BUFFER_SIZE) {
                        o_file.write(wbuffer, wlength);
                        memset(wbuffer, 0, BUFFER_SIZE + 1);
                        wlength = 0;
                    }
                }
            } else if (skip && curr_chr != '>') {
                chromosome_name += curr_chr;
            }
            rlength++;
        }
    }
    if (o_file.is_open()) {
        o_file.write(wbuffer, wlength);
        o_file.close();
    }
}

vector<string> GenomeCleaner::get_chromosomes() {
    return chromosomes;
}

string GenomeCleaner::chromosome_file_path(int id) {
    return genome_dir + PATH_SEPERATOR + CHROMOSOME_PREFIX + pad(id, PAD_WIDTH);
}