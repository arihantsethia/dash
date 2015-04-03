#include "headers/file_buffer.h"
//TODO : see if buf initialization with ' ' is fine
FileBuffer::FileBuffer(string file_path, size_t len = SEED_LEN) : len(len), buf(len, ' '), file_path(file_path) {
    s = 0;
    e = -1;

    try {
        fin.open(file_path);
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file opening:" << file_path << endl;
    }
}

FileBuffer::~FileBuffer() {
    try {
        if(fin.is_open()) {
            fin.close();
        }
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file closing:" << file_path << endl;
    }
}
//TODO : correct has_next() function
bool FileBuffer::has_next() {
    return !fin.eof();
}
//TODO : optimize file IO by using buffered reader.
//TODO : correct next() function
string FileBuffer::next() {
    string seed(len,' '); //check if ' ' is valid
    int idx = 0;
    char c;
    try {
        while (!fin.eof() && idx != len) {
            fin.get(c);
            seed[idx] = c;
            idx++;

            e++;
            buf[e % len] = c;
            if (e - s == 5) s++;
        }

        //check if seed is indeed full
        //if seed is not full, use the most recent chars from buffer
        if (idx != len) {
            for (int b = s; !(idx == len || b > e); ++b, ++idx) {
                seed[idx] = buf[b % len];
            }
        }

        return seed;
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file IO:" << file_path << endl;
    }
}