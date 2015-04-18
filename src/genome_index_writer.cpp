#include "headers/genome_index_writer.h"

GenomeIndexWriter::GenomeIndexWriter(int seed_len) : GenomeIndex(seed_len) {}

void GenomeIndexWriter::write_index(string filename) {
    vector<string> tokens = tokenize(filename, "_");
    int chromo_id = stoi(tokens.back());

    cout<<"Processing Chromosome : "<<chromo_id<<endl;

    db.use_table(chromo_id);

    FileBuffer fb(filename);
    t_value pos = 0;
    int x = 0;
    vector<key_value> data;
    while (fb.has_next()) {
        cout<<"Reading chromosome..."<<endl;
        key_value_map data;
        ll counter = JOB_LEN;
        while (fb.has_next() && counter--) {
            t_key key = hash.get_hash(fb.next());
            data[key].push_back(pos);
            pos++;
        }
        db.put(data);
        cout<<"Done : "<<(++x)*5<<"%"<<endl;
    }
}

void GenomeIndexWriter::close_index() {
    db.close_database();
}
