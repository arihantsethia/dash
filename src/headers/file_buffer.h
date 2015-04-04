#ifndef FILE_BUFFER
#define FILE_BUFFER
#include "generic.h"
#include "constants.h"
class FileBuffer
{
private:
    string file_path;
    char* buf;
    size_t s, e;
    size_t len;
    ifstream fin;
public:
    FileBuffer(string file_path, size_t len = SEED_LEN);

    string next();
    bool has_next();
    ~FileBuffer();
};
#endif