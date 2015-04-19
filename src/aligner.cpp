#include "headers/aligner.h"

Aligner::Aligner(int num_chromo, IndexProperties ip, int seed_len, int threads, int ed_threshold, int conf) : seed_len(seed_len), threads(threads), ed_threshold(ed_threshold), conf(conf), num_chromo(num_chromo), ip(ip) {}

void Aligner::align(vector<fq>& reads) {
    //returns 1 indexed vector. See function definition
    GenomeIndexReader gir(num_chromo, seed_len, threads);
    vector<vector<unordered_set<t_value> > > reads_chromo_pos_set = gir.get_positions(reads);

    FileReader fr(num_chromo);
    ReadAligner read_aligner(num_chromo, ed_threshold, conf);
    for (int r = 0; r < reads.size(); ++r) {
        read_aligner.align_read(reads[r].seq, reads_chromo_pos_set[r]);
        reads[r].XS = "XS:i:" + istr(read_aligner.alignment_status);
        if (read_aligner.alignment_status == ReadAligner::CONFIDENT) {
            reads[r].rname = ip.get_property(CHROMOSOME_PREFIX + pad(read_aligner.best_chromo, PAD_WIDTH));
            reads[r].pos =  read_aligner.best_pos + stoi(ip.get_property(CHROMOSOME_OFFSET + pad(read_aligner.best_chromo, PAD_WIDTH)));
            reads[r].NM = "NM:i:" + istr(read_aligner.d_best);
            reads[r].AS = "AS:i:" + istr(read_aligner.d_best);
        } else {
            reads[r].pos = 0;
            reads[r].NM = "*";
            reads[r].AS = "*";
        }
    }
}
