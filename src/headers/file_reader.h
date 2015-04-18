#ifndef FILE_READER
#define FILE_READER

#include "constants.h"
using namespace std;

class FileReader
{
private:
    string chromosome_path;
    vector<ifstream> fin_array;
public:
    FileReader();
    string get_DNA_sequence(int id, t_value pos, int read_len);
    ~FileReader();

};
#endif