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

/**
 * Writes the given byte's bits into the low order bits of consecutive
 * bytes in the file.
 *
 * @param byte The byte whose bits should be written to the file
 */
void StegImage::put(char byte) {
    // Used if the bitDepth is larger than 8 to skip bytes in order to get to
    // the byte with the low order bit.
    const int BYTES_TO_SKIP = (bitDepth / CHAR_BIT) - 1;
    // Depth is maximum of 8, since we are reading a byte at a time and
    // bytes are skipped if the bitDepth is large
    size_t depth = bitDepth;
    if (depth > CHAR_BIT) {
        depth = CHAR_BIT;
    }
    // Loop until all bits in the character have been embedded.
    size_t bitsPut = 0;
    while (bitsPut < CHAR_BIT) {
        // Skip bytes in the file to find the low order bit if necessary
        if (BYTES_TO_SKIP > 0) {
            file.seekg(BYTES_TO_SKIP, ios::cur);
        }
        // Read the byte containing low order bit(s)
        char data = file.peek();
        for (size_t bitPos = CHAR_BIT; bitPos - depth > -1 && bitsPut < CHAR_BIT; bitPos -= depth, byte >>= 1) {
            // Shift left to get a zero bit in the correct bit position
            char clearBitMask = 0xFE << (bitPos - depth);
            // Create a mask for pos to fill one's where zeros got filled
            char correctionMask = 0xFF >> (CHAR_BIT - bitPos + depth);
            // Fill with ones where zeros got filled, except for the bit position
            clearBitMask |= correctionMask;
            // Move the low order bit of the byte to the bit position
            char mask = (byte & 1) << (bitPos - depth);
            // Clear the bit in data, and then replace it with the low order bit
            data = (data & clearBitMask) | mask;
            ++bitsPut;
        }
        // Replace the byte in the file with the bit embedded byte
        file.write(&data, sizeof(char));
    }
}

bool StegImage::messageFits(string) {
    // TODO
    return false;
}
