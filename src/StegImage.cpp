/*
 * Author: Jacob Curtis, Joseph Perex, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 3/26/2020
 */

#include <climits>
#include "StegImage.h"

StegImage::StegImage(string) {
    // TODO
}

char StegImage::get() {
    // TODO
    return '\0';
}

/**
 * Writes the given byte's bits into the low order bits of consecutive
 * bytes in the file.
 *
 * @param byte The byte whose bits should be written to the file
 */
void StegImage::put(char byte) {
    for (size_t i = 0; i < CHAR_BIT; ++i, byte >>= 1) {
        char ch = file.peek();
        ch = (ch ^ 1) | (byte & 1);
        file.write(reinterpret_cast<char*>(ch), sizeof(char));
    }
}

bool StegImage::messageFits(string) {
    // TODO
    return false;
}
