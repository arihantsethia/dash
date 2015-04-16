#include "headers/genome_index_writer.h"

GenomeIndexWriter::GenomeIndexWriter(int seed_len) : GenomeIndex(seed_len) {}

GenomeIndexWriter::~GenomeIndexWriter() {}

void GenomeIndexWriter::write_index(string filename) {
    vector<string> tokens = tokenize(filename, "_");
    int chromo_id = stoi(tokens.back());
    db.use_table(chromo_id);

    FileBuffer fb(filename);
    t_value pos = 0;
    int x = 0;
    while (fb.has_next()) {
        key_value_map data;
        ll counter = JOB_LEN;
        while (fb.has_next() && counter--) {
            t_key key = hash.get_hash(fb.next());
            data[key].push_back(pos);
            pos++;
        }
        db.put(data);
    }
}
