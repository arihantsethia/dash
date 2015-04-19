#ifndef READ_ALIGNER
#define READ_ALIGNER

#include "constants.h"
#include "file_reader.h"
#include "generic.h"
using namespace std;

extern const int INF;

class ReadAligner {

private:
    FileReader fr;
    int levenshtein_distance(const string &s1, const string &s2);
    int ukonnen_distance(int n, int m, const string& s1, const string& s2, int t, int a = 1, int b = 1);
    inline int absolute(int a);
    int ed_threshold;
    int conf;

public:
    enum AlignmentStatus {
        CONFIDENT,
        MULTIPLE_HITS,
        NO_HITS
    };

    AlignmentStatus alignment_status;
    int d_best, num_chromo;
    t_value best_pos;
    int best_chromo;

    void align_read(string& read, vector < unordered_set<t_value> >& chromo_pos);
    ReadAligner(int num_chromo, int ed_threshold = MAX_EDIT_DIST, int conf = CONFIDENCE_THRESHOLD);
};
#endif