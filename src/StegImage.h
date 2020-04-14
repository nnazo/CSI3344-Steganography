/*
 * Author: Jacob Curtis, Joseph Perex, Josh Huertas, Joseph Yu, Micah Schiewe
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

#include "PNGConstants.h"

using namespace std;

// Output file name
#define IMG_NAME "result.png"

class StegImage {
private:
    fstream file;
    char bitDepth;
    bool inError;
    bool hasAlpha;

    size_t width, height;

    ofstream writer;
    bool writeReady;

public:
    StegImage(string);

    ~StegImage() {
        if (file.is_open()) {
            file.close();
        }
        if (writer.is_open())
            writer.close();
    }

    void prepToWrite();

    char get();

    void put(char);

    bool messageFits(string);
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


#endif // STEG_IMAGE_H
