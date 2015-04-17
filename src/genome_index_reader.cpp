#include "headers/genome_index_reader.h"

GenomeIndexReader::GenomeIndexReader(int seed_len) : GenomeIndex(seed_len) {}

vector<vector<unordered_set<t_value> > > GenomeIndexReader::get_positions(vector<string>& reads) {
    vector<vector<t_key> > seeds(reads.size());
    unordered_set<t_key> seeds_set;
    seeds_set.reserve(reads.size());    //reserving some space so as to reduce resizing a bit
    Seeds s(seed_len);
    for (int i = 0; i < reads.size(); i++) {
        vector<t_key> temp = s.get_seeds(reads[i]);
        seeds[i] = temp;
        seeds_set.insert(temp.begin(), temp.end());
    }
    //CAUTION: 1-indexed vector
    vector < key_value_map > chromo_map_seeds_pos(NUM_CHROMOSOMES + 1);
    for (int i = 1; i <= NUM_CHROMOSOMES; ++i) {
        db.use_table(i);
        chromo_map_seeds_pos[i] = db.get(seeds_set);
        // TODO: implement h_max threshold in the batch_set function. Read more at OneNote
        db.close_table();
    }

    vector<vector<unordered_set<t_value> > > reads_chromo_pos_set(reads.size(), vector<unordered_set<t_value> > (NUM_CHROMOSOMES + 1));

    for (int r = 0; r < reads.size(); ++r) {
        // seperate case for last seed index
        int s_idx;
        for (s_idx = 0; s_idx < seeds[r].size() - 1; ++s_idx) {
            t_key seed_temp = seeds[r][s_idx];
            for (int c = 1; c <= NUM_CHROMOSOMES; ++c) {
                for (int p = 0; p < chromo_map_seeds_pos[c][seed_temp].size(); ++p) {
                    // correct positions by compensating for the offset
                    int pos = chromo_map_seeds_pos[c][seed_temp][p];
                    // save read pos only if the following quantity >= 0
                    int offset_pos = pos - s_idx * seed_len;
                    // if (pos - s_idx * seed_len >= 0 ) {
                        reads_chromo_pos_set[r][c].insert(max(0,offset_pos));
                        // reads_chromo_pos_set[r][c].insert(pos - s_idx * seed_len);
                    // }
                }
            }
        }

        // for last seed
        t_key seed_temp = seeds[r][s_idx];
        int mod = reads[r].size() % seed_len;
        int val = reads[r].size() / seed_len - 1;
        for (int c = 1; c <= NUM_CHROMOSOMES; ++c) {
            for (int p = 0; p < chromo_map_seeds_pos[c][seed_temp].size(); ++p) {
                // correct positions by compensating for the offset
                int pos = chromo_map_seeds_pos[c][seed_temp][p];
                // save seed pos only if the following quantity >= 0
                int offset_pos = pos - val * seed_len - mod;
                // if (pos - val * seed_len - mod >= 0 ) {
                    reads_chromo_pos_set[r][c].insert(max(0,offset_pos));
                    // reads_chromo_pos_set[r][c].insert(pos - val * seed_len - mod);
                // }
            }
        }
    }

    return reads_chromo_pos_set;    //CAUTION: 1-indexed vector at second level

}

