#include "headers/file_buffer.h"

FileBuffer::FileBuffer(string file_path, size_t len) : len(len), file_path(file_path) {
    s = 0;
    e = len - 1;
    pos = 0;

    buf = new char[len];

    ifstream fin;
    try {
        fin.open(file_path);
        getline(fin, chromo);
        for (int i = 0; i < len; ++i) {
            buf[i] = chromo[i];
        }
        
        pos = len;
        // fin.read(buf, len);
        fin.close();
    }
    catch (ifstream::failure e) {
        cerr << "Exception in file opening:" << file_path << endl;
    }
}

FileBuffer::~FileBuffer() {
    delete[] buf;
    // if (fin.is_open()) {
    //     fin.close();
    // }
}
bool FileBuffer::has_next() {
    return pos != chromo.size();
}
string FileBuffer::next() {
    // if(filebuf_pos == FILEBUF_SIZE) {

    // }


    string seed(len, ' ');
    for (int i = s; i <= e; ++i) {
        seed[i - s] = buf[i % len];
    }

    e++;
    // fin.get(buf[e % len]);
    buf[e % len] = chromo[pos];
    pos++;
    if (e - s == len) s++;
    return seed;
}
