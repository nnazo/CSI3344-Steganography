/*
 * Author: Jacob Curtis, Joseph Perez, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 3/26/2020
 */

#ifndef STEG_IMAGE_H
#define STEG_IMAGE_H

#include <fstream>
#include <climits>
#include <cassert>
#include <vector>
#include <string>
#include <cmath>

#include "PNGConstants.h"

using namespace std;

class StegImage {
private:
    fstream file;
    char bitDepth;
    bool inError;

    size_t width, height;

    vector<char> buffer;
    streampos start;
    string filename;

    template <class T>
    T findNextT();

public:
    StegImage(string);

    ~StegImage() {
        if (file.is_open() && buffer.size() > 0)
            flushAndClose();
        else if (file.is_open())
            file.close();
    }

    char get();

    void put(char);

    bool messageFits(const string&);

    void flushAndClose();

    void seekNextIdat();
};

bool find(fstream&, const string&);

template <class T>
T flip(T toFlip) {
    T buffer = 0;
    const int MAX = sizeof(T);

    for (int i = 0; i < MAX; i++) {
        buffer <<= 8;
        buffer |= (toFlip & 0xFF);
        toFlip >>= 8;
    }

    return buffer;
}

template <class T>
T StegImage::findNextT() {
    string tmp;
    char c;

    // Skip header material to find the next item
    do {
        c = file.get();

        // Skip comments
        if (c == '#') {
            getline(file, tmp);
            c = ' ';
        }
    } while (iswspace(c));
    file.unget();

    // Parse result
    T result;
    file >> result;
    return result;
}

#endif // STEG_IMAGE_H
