//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_BUFFEREDINPUTSTREAM_HPP
#define PYTHONVM_BUFFEREDINPUTSTREAM_HPP

#include<stdio.h>

#define BUFFER_LEN 256

class BufferedInputStream {
private:
    FILE *fp;
    char szBuffer[BUFFER_LEN];
    unsigned short index;

public:
    BufferedInputStream(char const *filename) {
        fp = fopen(filename, "rb");
        fread(szBuffer, BUFFER_LEN * sizeof(char), 1, fp);
        index = 0;
    }

    ~BufferedInputStream() {
        close();
    }

    char read() {
        if (index < BUFFER_LEN)
            return szBuffer[index++];
        else {
            index = 0;
            fread(szBuffer, BUFFER_LEN * sizeof(char), 1, fp);
            return szBuffer[index++];
        }
    }

    int read_int() {
        int b1 = read() & 0xff;
        int b2 = read() & 0xff;
        int b3 = read() & 0xff;
        int b4 = read() & 0xff;

        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    double read_double() {
        char t[8];
        for (int i = 0; i < 8; i++) {
            t[i] = read();
        }

        return *(double *) t;
    }

    void unread() {
        index--;
    }

    void close() {
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }
    }
};


#endif //PYTHONVM_BUFFEREDINPUTSTREAM_HPP
