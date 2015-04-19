#include "headers/SAM.h"

SAM::SAM() {}

SAM::~SAM() {}

void SAM::read_file(string fq_name) {
    ifstream fq_read (cstr(fq_name));
    string str_fq;
    int cnt = 0;
    fq new_read;
    new_read.flags = MATCH_FLAGS;
    while (getline(fq_read, str_fq)) {
        cnt %= 4;
        if (cnt == 0) {
            new_read.seq_ID = str_fq;
            new_read.seq_ID[new_read.seq_ID.size() - 2] = '\0';
        }
        else if (cnt == 1)
            new_read.seq = str_fq;
        else if (cnt == 3)
            new_read.quality_string = str_fq;
        cnt++;
        if (cnt == 4)
            reads.push_back(new_read);
    }
}

void SAM::write_sam(string sam_write) {
    ofstream sam (cstr(sam_write));
    sam << HEADER << CO;
    for (int i = 0; i < (this->reads).size(); i++) {
        //cout << this->reads[i].seq_ID << " " << this->reads[i].seq << " " << this->reads[i].quality_string << endl;
        sam << this->reads[i].seq_ID << "\t" << this->reads[i].flags << "\t" << this->reads[i].rname << "\t" << this->reads[i].pos << "\t" << this->reads[i].mapq << "\t" << this->reads[i].cigar << "\t" << this->reads[i].seq << "\t" << this->reads[i].quality_string << "\t" << this->reads[i].NM << "\t" << this->reads[i].AS << "\t" << this->reads[i].MD << "\t" << this->reads[i].XS << endl;
    }
    //chr1_188832065_188832164_0:3:1_0:1:1_1  0       chr1    188832065       60      76M1D24M        *       0       0       GAATAAAAAAATTCTTGATGTTATTTTAATCTTCTTAAGTTGTTATGTCTTGTTTTGTGACCAAGTATACACTATGCCCAGAGAATGTTGTTTTTACTCT    IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII    NM:i:4  MD:Z:3A8G63^C13C10      AS:i:79 XS:i:20
    sam.close();
}