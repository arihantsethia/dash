#include "headers/genome_index_writer.h"

GenomeIndexWriter::GenomeIndexWriter(int seed_len) : GenomeIndex(seed_len) {
    // IndexProperties I;
    // string seed_len_str = I.get_property(SEED_LEN_PROP);
    // seed_len = stoi(seed_len_str);
}

void GenomeIndexWriter::write_index(string filename) {
    vector<string> tokens = tokenize(filename, "_");
    int chromo_id = stoi(tokens.back());
    db.use_table(chromo_id);

    FileBuffer fb(filename);
    string seed;
    t_value pos = 0;
    int x;
    vector<key_value> data;
    while (fb.has_next()) {
        data.clear();
        data.reserve(JOB_LEN);
        while (fb.has_next() && data.size() < JOB_LEN) {
            seed = fb.next();
            t_key key = hash.get_hash(seed);
            data.push_back(make_pair(key, pos));
            pos++;
        }
        cout<<x++<<"Inserting : "<<pos<<endl;
        db.put(data);
        cout<<"Done"<<endl;
    }
}