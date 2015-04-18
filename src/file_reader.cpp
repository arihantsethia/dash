#include "headers/file_reader.h"

FileReader::FileReader() : fin_array(NUM_CHROMOSOMES + 1) {
    try {
        for (int i = 1; i <= NUM_CHROMOSOMES; i++ ) {
            chromosome_path = GENOME_PATH + CHROMOSOME_PREFIX + pad(i, PAD_WIDTH);
            cout << chromosome_path << endl;
            fin_array[i].open(chromosome_path, ifstream::binary);
            if (!fin_array[i].is_open()) {
                cout << "File not found:" << chromosome_path << endl;
            }
        }
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file opening:" << endl;
    }
}

string FileReader::get_DNA_sequence(int id, t_value pos, int read_len) {
    fin_array[id].seekg(pos, fin_array[id].beg);
    cout << pos << " " << read_len << " " << endl;
    char* seq = new char[read_len + 1];
    memset(seq, 0, read_len + 1);

    string dna_seq = "";
    fin_array[id].read(seq, read_len);
    dna_seq.assign(seq);

    fin_array[id].clear();
    delete[] seq;
    return dna_seq;
}

FileReader::~FileReader() {
    for (int i = 0; i <= NUM_CHROMOSOMES; ++i)
    {
        fin_array[i].close();
    }
}
