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

StegImage::StegImage(string) {
    // TODO
}

char StegImage::get() {
    char result = '\0';
    int bitsRemaining = CHAR_BIT;

    char buffer;
    int bitCntr;

    // Create a mask to isolate the high-order bit
    char mask = 0x1;
    mask <<= CHAR_BIT - 1;
    mask = ~mask;

    // Compute whether a certain number of bytes need to be skipped.
    const int BYTES_TO_SKIP = (bitDepth / CHAR_BIT) - 1;
    const int DEPTH = bitDepth > CHAR_BIT ? CHAR_BIT : bitDepth;

    while (bitsRemaining > 0) {
        // Read byte with encoded data
        if (BYTES_TO_SKIP > 0) {
            file.seekp(BYTES_TO_SKIP, ios::cur);
            file.seekg(BYTES_TO_SKIP, ios::cur);
        }
        buffer = file.get();

        // Prepare to read
        buffer <<= bitDepth - 1;
        bitCntr = CHAR_BIT - bitDepth + 1;

        // Read bits out of this byte
        do {
            // Pre-decrement, then extract the bit
            bitsRemaining--;
            result |= (buffer & mask) >> bitsRemaining;

            // Prepare to extract the next bit
            buffer <<= bitDepth;
            bitCntr -= bitDepth;
        } while (bitCntr >= 0);
    }

    return result;
}

void StegImage::put(char) {
    // TODO
}

bool StegImage::messageFits(string) {
    // TODO
    return false;
}
