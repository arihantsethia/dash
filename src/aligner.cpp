#include "headers/aligner.h"
Aligner::Aligner(int seed_len, int threads, int ed_threshold, int conf, int num_chromo) : seed_len(seed_len), threads(threads), ed_threshold(ed_threshold), conf(conf), num_chromo(num_chromo) {}

void Aligner::align(vector<string>& reads) {
    //returns 1 indexed vector. See function definition
    GenomeIndexReader gir(seed_len, threads);
    vector<vector<unordered_set<t_value> > > reads_chromo_pos_set = gir.get_positions(reads);

    FileReader fr(num_chromo);
    ReadAligner read_aligner(ed_threshold, conf, num_chromo);
    for (int r = 0; r < reads.size(); ++r) {
        cout << "Read: " << reads[r] << " ..." << endl;
        read_aligner.align_read(reads[r], reads_chromo_pos_set[r]);
        cout << read_aligner.alignment_status << endl;
        cout << read_aligner.d_best << "\t" << read_aligner.best_pos << "\t" << read_aligner.best_chromo << "\t" << endl;
    }
}
