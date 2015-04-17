#include "headers/aligner.h"
//TODO remove the seed_len initialization in intializer list. Uncomment the IndexProp thing
Aligner::Aligner(int seed_len) : seed_len(seed_len) {
    cout<<seed_len<<endl;
    // IndexProperties I;
    // string seed_len_str = I.get_property(SEED_LEN_PROP);
    // seed_len = stoi(seed_len_str);
}

void Aligner::align(vector<string> reads) {
    //returns 1 indexed vector. See function definition
    GenomeIndexReader gir(seed_len);
    vector<vector<unordered_set<t_value> > > reads_chromo_pos_set = gir.get_positions(reads);

    FileReader fr;
    ReadAligner read_aligner;
    for (int r = 0; r < reads.size(); ++r) {
        cout << "Read: " << reads[r] << " ..." << endl;
        read_aligner.align_read(reads[r], reads_chromo_pos_set[r]);
        cout<<read_aligner.alignment_status<<endl;
        cout<<read_aligner.d_best<<"\t"<<read_aligner.best_pos<<"\t"<<read_aligner.best_chromo<<"\t"<<endl;
        // for (int c = 1; c <= NUM_CHROMOSOMES; ++c) {
        //     cout << "#" << c << endl;
        //     for (auto& p : reads_chromo_pos_set[r][c]) {
        //         string dna_seq = fr.get_DNA_sequence(c, p, reads[r].size());

        //         if (dna_seq == "") {
        //             continue;
        //         }

        //         cout << dna_seq << endl;
        //         // reads[r].size() is guarenteed to be greater than dna_seq.size()
        //         cout << ukonnen_distance(reads[r].size(), dna_seq.size(), reads[r], dna_seq) << endl;
        //     }
        // }
    }
}
