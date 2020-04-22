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


#include "PNGConstants.h"

using namespace std;

class StegImage {
private:
    fstream file;
    char bitDepth;
    bool inError;
    bool hasAlpha;

    size_t width, height;

    queue<char> buffer;
    streampos start;
    string filename;

public:
    StegImage(string);

    ~StegImage() {
        if (file.is_open())
            file.close();
    }

    char get();

    void put(char);

    bool messageFits(const string&);

    void flushAndClose(string);

    void seekNextIdat();
};

bool find(fstream&, const string&);


#endif // STEG_IMAGE_H
