#include "headers/generic.h"
#include "headers/constants.h"
#include "headers/util.h"
#include "headers/database.h"
#include "headers/genome_cleaner.h"
#include "headers/file_buffer.h"
#include "headers/genome_index_writer.h"
#include "headers/genome_index_reader.h"
#include "headers/file_reader.h"
#include "headers/aligner.h"
#include "headers/index_properties.h"
#include "headers/argument_parser.h"

using namespace std;

void initialize_dash_dirs() {
    mkdir(cstr(PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(GENOME_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(cstr(DB_PATH), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

vector<string> get_reads(string in) {
    vector<string> reads;
    ifstream fin;
    try {
        fin.open(in);
        string line;
        while (getline(fin, line)) {
            reads.push_back(line);
        }
        fin.close();
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file reading" << in << endl;
        if (fin.is_open()) {
            fin.close();
        }
    }
    return reads;
}

void indexer(string genome, int seed_len, int threads) {
    IndexProperties ip;
    GenomeCleaner g(genome);
    g.clean();

    vector<string> chromosomes = g.get_chromosomes();
    vector<t_value> offsets = g.get_offsets();
    for (int i = 1; i <= chromosomes.size(); i++) {
        ip.put_property(CHROMOSOME_PREFIX + pad(i, PAD_WIDTH) , chromosomes[i - 1]);
        ip.put_property(CHROMOSOME_OFFSET + pad(i, PAD_WIDTH) , istr(offsets[i - 1]));
    }

    GenomeIndexWriter giw(seed_len, threads);
    for (int i = 1; i <= chromosomes.size(); ++i) {
        giw.write_index(GENOME_PATH + CHROMOSOME_PREFIX + pad(i, PAD_WIDTH));
    }

    giw.close_index();
    ip.put_property(SEED_LEN_PROP, to_string(seed_len));
    ip.put_property(NUM_CHROMO_PROP, to_string(chromosomes.size()));
    ip.write_file();
}

void aligner(string in, string out, int threads, int edit_dist, int conf, string usage) {
    IndexProperties ip;
    ip.read_file();
    int seed_len_read = stoi(ip.get_property(SEED_LEN_PROP));
    int num_chromo = stoi(ip.get_property(NUM_CHROMO_PROP));

    vector<string> reads = get_reads(in);

    Aligner a(num_chromo, seed_len_read, threads, edit_dist, conf);
    a.align(reads);
    // use this ip.put_property(CHROMOSOME_OFFSET + pad(i, PAD_WIDTH) , istr(offsets[i - 1]));
    // and add
}

void parse_error(ArgumentParser &ap) {
    ap.print_usage();
    exit(1);
}

int main(int argc, char *argv[]) {
    ArgumentParser ap;
    ap.parse_args(argc, argv);

    initialize_dash_dirs();

    if (ap.index) {
        if (ap.genome == "" || ap.seed_len <= 0 || ap.threads <= 0) {
            parse_error(ap);
        } else {
            indexer(ap.genome, ap.seed_len, ap.threads);
        }
    }
    else if (ap.align) {
        if (ap.out == "" || ap.in == "" || ap.threads <= 0 || ap.edit_dist < 0 \
                || ap.conf <= 0 || !(ap.usage == "master" || ap.usage == "client")) {
            parse_error(ap);
        } else {
            //TODO: what to do with distributed option?
            //TODO: what to do with mode?
            aligner(ap.in, ap.out, ap.threads, ap.edit_dist, ap.conf, ap.usage);
        }
    }
    else {
        parse_error(ap);
    }
    return 0;
}