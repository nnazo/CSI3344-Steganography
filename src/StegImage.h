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

using namespace std;

class StegImage {
private:
    fstream file;
    int bitDepth;
    bool inError;

    int width, height;

public:
    StegImage(string);

    ~StegImage() {
        if (file) file.close();
    }

    char get();

    void put(char);

    bool messageFits(string);
};

#endif // STEG_IMAGE_H
