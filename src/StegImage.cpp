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
        // Read size
        file.read((char*)&width, DIM_WIDTH);
        file.read((char*)&height, DIM_WIDTH);

        // Read bit depth and color code; detect alpha field
        file.read(&bitDepth, DEPTH_TYPE_WIDTH);
        file.read(&clrCode, DEPTH_TYPE_WIDTH);
        hasAlpha = (clrCode & ALPHA_DETECT_MASK) != 0;

        // Second check: not palette based
        inError = clrCode != PALETTE;
    }

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
    while (i < searchString.size() && in) {
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
