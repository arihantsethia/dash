#include "headers/genome_index_reader.h"

GenomeIndexReader::GenomeIndexReader(int num_chromo, int seed_len, int threads) : GenomeIndex(seed_len, threads), num_chromo(num_chromo) {}

vector<vector<unordered_set<t_value> > > GenomeIndexReader::get_positions(vector<fq>& reads) {
    vector<vector<t_key> > seeds(reads.size());
    vector<t_key> seeds_vec;
    seeds_vec.reserve(reads.size());
    Seeds s(seed_len);
    for (int i = 0; i < reads.size(); i++) {
        vector<t_key> temp = s.get_seeds(reads[i].seq);
        seeds[i] = temp;
        seeds_vec.insert(seeds_vec.end(), temp.begin(), temp.end());
    }

    // removing duplicate seeds
    sort( seeds_vec.begin(), seeds_vec.end() );
    seeds_vec.erase( unique( seeds_vec.begin(), seeds_vec.end() ), seeds_vec.end() );
    unordered_map<t_key, int> seed_map;
    for (int i = 0; i < seeds.size(); ++i) {
        for (int j = 0; j < seeds[i].size(); j++) {
            auto low = lower_bound (seeds_vec.begin(), seeds_vec.end(), seeds[i][j]);
            seed_map[seeds[i][j]] = low - seeds_vec.begin();
        }
    }

    vector < vector< vector<t_value> > > chromo_map_seeds_pos(num_chromo + 1);
    for (int i = 1; i <= num_chromo; ++i) {
        db.use_table(i);
        chromo_map_seeds_pos[i] = db.get(seeds_vec);
    }

    vector<vector<unordered_set<t_value> > > reads_chromo_pos_set(reads.size(), vector<unordered_set<t_value> > (num_chromo + 1));

    for (int r = 0; r < reads.size(); ++r) {
        // seperate case for last seed index
        int s_idx, mapped_s_idx;
        for (s_idx = 0; s_idx < seeds[r].size() - 1; ++s_idx) {
            t_key seed_temp = seeds[r][s_idx];
            mapped_s_idx = seed_map[seed_temp];
            for (int c = 1; c <= num_chromo; ++c) {
                for (int p = 0; p < chromo_map_seeds_pos[c][mapped_s_idx].size(); ++p) {
                    // correct positions by compensating for the offset
                    int pos = chromo_map_seeds_pos[c][mapped_s_idx][p];
                    // save read pos only if the following quantity >= 0
                    int offset_pos = pos - s_idx * seed_len;
                    reads_chromo_pos_set[r][c].insert(max(0, offset_pos));
                }
            }
        }

        // for last seed
        t_key seed_temp = seeds[r][s_idx];
        mapped_s_idx = seed_map[seed_temp];
        int mod = reads[r].seq.size() % seed_len;
        int val = reads[r].seq.size() / seed_len - 1;
        for (int c = 1; c <= num_chromo; ++c) {
            for (int p = 0; p < chromo_map_seeds_pos[c][mapped_s_idx].size(); ++p) {
                // correct positions by compensating for the offset
                int pos = chromo_map_seeds_pos[c][mapped_s_idx][p];
                // save seed pos only if the following quantity >= 0
                int offset_pos = pos - val * seed_len - mod;
                reads_chromo_pos_set[r][c].insert(max(0, offset_pos));
            }
        }
    }

    return reads_chromo_pos_set;

}

