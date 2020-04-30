/*
 * Author: Jacob Curtis, Joseph Perez, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: 5/1/2020
 * Date Created: 3/26/2020
 * Date Last Modified: 4/30/2020
 */

#ifndef STEG_IMAGE_H
#define STEG_IMAGE_H

#include <fstream>
#include <climits>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Identifies the PPM file type we need
#define MAGIC_NUMBER_STRING_LOWER "p6"
#define MAGIC_NUMBER_STRING_UPPER "P6"

/*
 * Class that handles opening and embedding information in or extracting
 * information from a PBP image.
 */
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

    ~StegImage();

    char get();

    void put(char);

    bool messageFits(streamoff);

    void flushAndClose(string);

    bool isInError();
};

bool find(fstream&, const string&);

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
