#include "headers/read_aligner.h"

const int INF = numeric_limits<int>::max();

ReadAligner::ReadAligner(int ed_threshold, int conf, int num_chromo): ed_threshold(ed_threshold), conf(conf), num_chromo(num_chromo) {}

/*
* for the given read, finds the best, second best alignments
* based on thresholds, returns whether read maps ambiguously/unambiguously/no hit
* uses ukonnen algo for edit dist based on max tolerable edit dist.
*/
void ReadAligner::align_read(string& read, vector < unordered_set<t_value> >& chromo_pos) {
    int d_curr = 0;                 //edit dist for curr pos
    int d_limit = ed_threshold;    //cutoff to be used for Ukonnen algo
    int d_second = INF;             //second best edit dist.
    d_best = INF;                   //best edit dist. found so far
    best_pos = -1;                  //pos corresponding to best edit dist.
    best_chromo = -1;               //chromosome id corresponding to best edit dist.
    alignment_status = NO_HITS;

    for (int c = 1; c <= num_chromo; ++c) {
        cout << "#" << c << endl;
        for (auto& p : chromo_pos[c]) {
            string dna_seq = fr.get_DNA_sequence(c, p, read.size());

            if (dna_seq == "") {
                continue;
            }
            cout << "DNA: " << dna_seq << endl;

            if (d_best > ed_threshold) {
                d_limit = ed_threshold + conf - 1;
            }
            else if (d_second >= d_best + conf) {
                d_limit = d_best + conf - 1;
            }
            else {
                d_limit = d_best - 1;
            }
            // read.size() is guaranteed to be greater than dna_seq.size()
            d_curr = ukonnen_distance(read.size(), dna_seq.size(), read, dna_seq, d_limit);

            if (d_curr == -1) {
                continue;
            }

            cout << "ED=" << d_curr << endl;
            if (d_curr < d_best) {
                if (d_best != INF)
                    d_second = d_best;

                d_best = d_curr;
                best_pos = p;
                best_chromo = c;
            }
            else if (d_curr >= d_best && d_curr < d_second) {
                d_second = d_curr;
            }
            if (d_best < conf && d_second < d_best + conf) {
                //we have two hits within dist. conf and no better hit can be confident
                alignment_status = MULTIPLE_HITS;
                return;
            }
        }
    }

    if (d_best <= ed_threshold && d_second >= d_best + conf) {
        alignment_status = CONFIDENT;
    }
    else if (d_best <= ed_threshold) {
        alignment_status = MULTIPLE_HITS;
    }
    else {
        alignment_status = NO_HITS;
    }
    return ;
}

/*
* n >= m
* edit distance in O(max(m,n)*k) time, where k = max tolerable edit distance
* code borrowed from CodeChef problem Sereja and two strings by tanuj1
* a is the cost of inserting or deleting a char from s1
* b is the cost of replacing a char in s1 with another char
* t is the max tolerable edit distance.
* if actual edit distance exceeds t, function returns -1
*/
int ReadAligner::ukonnen_distance(int n, int m, const string& s1, const string& s2, int t, int a, int b )
{
    vector<vector<int> > dp(2, vector<int>(n + 1, 0));

    if (t < a * (n - m)) {
        return -1;
    }

    if (a == 0) {
        return 0;
    }

    if (b == 0) {
        return a * (n - m);
    }

    int p = (t - a * (n - m)) / (2 * a);
    //printf("p : %d\n",p);

    int i, j;
    vector<int>& last = dp[0];
    vector<int>& current = dp[1];

    for (i = 0 ; i <= n ; i++) {
        current[i] = last[i] = 10000000;
    }

    for (i = 0; i <= m; i++) {

        int init, end;
        init = (i - p) > 0 ? i - p : 0;
        end = n < i + n - m + p  ? n : i + n - m + p;
        current[0] = i;

        if (init != 0 ) {
            current[init] = (last[init - 1] + (s1[init] == s2[i] ? 0 : b)) < (last[init] + a) ? (last[init - 1] + (s1[init] == s2[i] ? 0 : b)) : (last[init] + a);
        }

        for (j = init + 1; j <= end ; j++) {
            int p1 = last[j - 1] + (s1[j] == s2[i] ? 0 : b);
            int p2 = last[j] + a;
            int min = p1 < p2 ? p1 : p2;
            min = min < (current[j - 1] + a) ? min : current[j - 1] + a;
            current[j] = min;
        }

        if ( end != n) {
            current[end] = (last[end - 1] + (s1[end] == s2[i] ? 0 : b)) < (current[end - 1] + a) ? (last[end - 1] + (s1[end] == s2[i] ? 0 : b)) : (current[end - 1] + a);
        }
        swap(last, current);
    }


    if (last[n] <= t)
        return last[n];
    else
        return -1;

}

inline int ReadAligner::absolute(int a) {
    if (a < 0) return (-1) * a;
    else return a;
}

/*
* edit distance c++ efficient implementation from
* http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance
*/
int ReadAligner::levenshtein_distance(const string &s1, const string &s2) {
    int s1len = s1.size();
    int s2len = s2.size();

    auto column_start = (decltype(s1len))1;

    auto column = new decltype(s1len)[s1len + 1];
    iota(column + column_start, column + s1len + 1, column_start);

    for (auto x = column_start; x <= s2len; x++) {
        column[0] = x;
        auto last_diagonal = x - column_start;
        for (auto y = column_start; y <= s1len; y++) {
            auto old_diagonal = column[y];
            auto possibilities = {
                column[y] + 1,
                column[y - 1] + 1,
                last_diagonal + (s1[y - 1] == s2[x - 1] ? 0 : 1)
            };
            column[y] = min(possibilities);
            last_diagonal = old_diagonal;
        }
    }
    auto result = column[s1len];
    delete[] column;
    return result;
}