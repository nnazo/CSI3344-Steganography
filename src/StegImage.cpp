/*
 * Author: Jacob Curtis, Joseph Perex, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 3/26/2020
 */

#include "StegImage.h"

StegImage::StegImage(string filename) : file(filename, ifstream::binary) {
    char clrCode;

    // First check: find the header chunk, if possible, in the file
    inError = !find(file, HEADER);

    // Get dimensions + bit depth
    if (!inError) {
        // Say that one three times fast.
        const size_t size_t_size = sizeof(size_t);
        file.read((char*)&width, size_t_size);
        file.read((char*)&height, size_t_size);
        file.read(&bitDepth, CHAR_BIT);
        file.read(&clrCode, CHAR_BIT);
    }

    // Second check: not palette based
    inError = clrCode != PALETTE;

    // Parse dimensions
    if (!inError) {
        if (width >= TIPPING_POINT)
            width = flip(width);
        if (height >= TIPPING_POINT)
            width = flip(width);
        inError = find(file, DATA);
    }
}

char StegImage::get() {
    // TODO
    return '\0';
}

void StegImage::put(char) {
    // TODO
}

bool StegImage::messageFits(string) {
    // TODO
    return false;
}

/*
 * Finds the specified searchString in the provided file, from the current
 * read position.
 *
 * Returns: whether the string was found.
 */
bool find(fstream& in, const string& searchString) {
    int i = 0;
    char ch;

    // Search for 'searchString'
    while (i < searchString.length && in) {
        ch = in.get();

        // Search for the next character in the stream
        if (ch != searchString[i])
            i = 0;
        else
            i++;
    }

    // Return whether the string was found
    if (in)
        return true;
    else
        return false;
}
