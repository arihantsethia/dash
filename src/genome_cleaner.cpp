#include "headers/genome_cleaner.h"

const int GenomeCleaner::BUFFER_SIZE = 1;//00 * 1024 * 1024;

GenomeCleaner::GenomeCleaner(string genome_file, string genome_dir): genome_dir(genome_dir), genome_file(genome_file) {
}
void GenomeCleaner::clean() {
    ifstream i_file (genome_file);
    ofstream o_file;
    bool skip = false;
    int chromosome_counter = 1;
    char* rbuffer = new char [BUFFER_SIZE];
    char* wbuffer = new char [BUFFER_SIZE];
    memset(wbuffer, 0, sizeof(wbuffer));
    size_t wlength = 0, rlength = 0;
    while (i_file) {
        memset(rbuffer, 0, sizeof(rbuffer));
        i_file.read(rbuffer, BUFFER_SIZE);
        rlength = 0;
        while (rbuffer[rlength]) {
            char curr_chr = rbuffer[rlength];
            if (curr_chr == '>') {
                skip = true;
                if (o_file.is_open()) {
                    o_file.write(wbuffer, wlength);
                    o_file.close();
                }
                memset(wbuffer, 0, sizeof(wbuffer));
                wlength = 0;
                o_file.open(chromosome_file_path(chromosome_counter++));
            }
            else if (curr_chr == '\n') {
                skip = false;
            }

            if (!skip && o_file.is_open()) {
                if (get_base(curr_chr)) {
                    wbuffer[wlength++] = get_base(curr_chr);
                    if (wlength == BUFFER_SIZE) {
                        o_file.write(wbuffer, wlength);
                        memset(wbuffer, 0, sizeof(wbuffer));
                        wlength = 0;
                    }
                }
            }
            rlength++;
        }
    }
    if (o_file.is_open()) {
        o_file.write(wbuffer, wlength);
        o_file.close();
    }
}

string GenomeCleaner::chromosome_file_path(int id) {
    return genome_dir + PATH_SEPERATOR + CHROMOSOME_PREFIX + pad(id, 3);
}