#include "headers/aligner.h"
Aligner::Aligner(int seed_len) : seed_len(seed_len) {}

void Aligner::align(vector<string>& reads) {
    //returns 1 indexed vector. See function definition
    GenomeIndexReader gir(seed_len);
    vector<vector<unordered_set<t_value> > > reads_chromo_pos_set = gir.get_positions(reads);

    FileReader fr;
    ReadAligner read_aligner;
    for (int r = 0; r < reads.size(); ++r) {
        cout << "Read: " << reads[r] << " ..." << endl;
        read_aligner.align_read(reads[r], reads_chromo_pos_set[r]);
        cout << read_aligner.alignment_status << endl;
        cout << read_aligner.d_best << "\t" << read_aligner.best_pos << "\t" << read_aligner.best_chromo << "\t" << endl;
    }
}
