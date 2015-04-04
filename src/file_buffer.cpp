#include "headers/file_buffer.h"
//TODO : see if buf initialization with ' ' is fine
FileBuffer::FileBuffer(string file_path, size_t len) : len(len), file_path(file_path) {
    s = 0;
    e = len - 1;

    buf = new char[len];

    try {
        fin.open(file_path);
        fin.read(buf, len);
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file opening:" << file_path << endl;
    }
}

FileBuffer::~FileBuffer() {
    delete[] buf;
    if (fin.is_open()) {
        fin.close();
    }
}
bool FileBuffer::has_next() {
    return !fin;
}
//TODO : optimize file IO by using buffered reader.
string FileBuffer::next() {
    string seed(len, ' ');
    for (int i = s; i <= e; ++i) {
        seed[i - s] = buf[i % len];
    }

    try {
        e++;
        fin.get(buf[e % len]);
        if (e - s == len) s++;
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file IO:" << file_path << endl;
    }

    return seed;
}
