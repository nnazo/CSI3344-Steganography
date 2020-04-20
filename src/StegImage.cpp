<<<<<<< HEAD
/*
 * Author: Jacob Curtis, Joseph Perez, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 3/26/2020
 */

#include <climits>
#include "StegImage.h"
#include <iostream>

StegImage::StegImage(string filename) : file(filename, fstream::in |
    fstream::out | fstream::binary), filename(filename) {
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
        inError = clrCode == PALETTE;
    }

    // Parse dimensions
    if (!inError) {
        if (width >= TIPPING_POINT)
            width = flip(width);
        if (height >= TIPPING_POINT)
            height = flip(height);

        // Ensure there is an IDAT chunk (error check), then seek its end.
        inError = !find(file, DATA);
    }

    // Save data section address
    if (!inError)
        start = file.tellg();
}

char StegImage::get() {
    char result = '\0';
    int bitsRemaining = CHAR_BIT;

    unsigned char buffer;
    int bitCntr;

    // Create a mask to isolate the high-order bit
    char mask = 0x1;
    mask <<= CHAR_BIT - 1;

    // Compute whether a certain number of bytes need to be skipped.
    const int BYTES_TO_SKIP = (bitDepth / CHAR_BIT) - 1;
    const int DEPTH = bitDepth > CHAR_BIT ? CHAR_BIT : bitDepth;

    // Prep to read
    streampos pos = file.tellg();
    file.sync();
    file.seekg(pos, ios::beg);

    while (bitsRemaining > 0) {
        // Read byte with encoded data
        if (BYTES_TO_SKIP > 0) {
            file.seekg(BYTES_TO_SKIP, ios::cur);
        }

        // Prepare to read
        file.read((char*)&buffer, sizeof(char));
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
    int depth = bitDepth;
    if (depth > CHAR_BIT) {
        depth = CHAR_BIT;
    }
    // Loop until all bits in the character have been embedded.
    char data;
    size_t bitsPut = 0, bitPos;

    // Prep to write
    while (bitsPut < CHAR_BIT) {
        // Skip bytes in the file to find the low order bit if necessary
        if (BYTES_TO_SKIP > 0) {
            file.seekg(BYTES_TO_SKIP, ios::cur);
        }
        // Read the byte containing low order bit(s)
        file.read(&data, 1);
        for (int bitPos = CHAR_BIT; bitPos - depth > -1 && bitsPut < CHAR_BIT; bitPos -= depth, byte >>= 1) {
            // Shift left to get a zero bit in the correct bit position
            char clearBitMask = 0xFE << (bitPos - depth);
            // Create a mask for pos to fill one's where zeros got filled
            unsigned char correctionMask = 0xFF >> (CHAR_BIT - bitPos + depth);
            // Fill with ones where zeros got filled, except for the bit position
            clearBitMask |= correctionMask;
            // Move the low order bit of the byte to the bit position
            char mask = (byte & 1) << (bitPos - depth);
            // Clear the bit in data, and then replace it with the low order bit
            data = (data & clearBitMask) | mask;
            ++bitsPut;
        }
        // Replace the byte in the file with the bit embedded byte
        buffer.push_back(data);
    }
}

bool StegImage::messageFits(const string &s) {
    return (s.length() * CHAR_BIT) <= (width*height*(hasAlpha ? 4 : 3));
}

void StegImage::flushAndClose() {
    // Create write handle
    file.close();
    file.clear();
    file.open(filename, ios::ate | ios::in | ios::out | ios::binary);
    file.seekp(start, ios::beg);
    assert(!file == false);

    // Write data
    for (char c : buffer)
        file.write(&c, 1);
    file.flush();
    file.close();
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
    if (in) {
        // Seek put head to the correct position, if needed
        in.seekp(in.tellg(), ios::beg);
        return true;
    } else
        return false;
}
=======
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
#include <iostream>

StegImage::StegImage(string filename) : file(filename, fstream::in |
    fstream::out | fstream::binary), filename(filename) {
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
        inError = clrCode == PALETTE;
    }

    // Parse dimensions
    if (!inError) {
        if (width >= TIPPING_POINT)
            width = flip(width);
        if (height >= TIPPING_POINT)
            height = flip(height);

        // Ensure there is an IDAT chunk (error check), then seek its end.
        inError = !find(file, DATA);
    }

    // Save data section address
    if (!inError)
        start = file.tellg();
}

char StegImage::get() {
    char result = '\0';
    int bitsRemaining = CHAR_BIT;

    unsigned char buffer;
    int bitCntr;

    // Create a mask to isolate the high-order bit
    char mask = 0x1;
    mask <<= CHAR_BIT - 1;

    // Compute whether a certain number of bytes need to be skipped.
    const int BYTES_TO_SKIP = (bitDepth / CHAR_BIT) - 1;
    const int DEPTH = bitDepth > CHAR_BIT ? CHAR_BIT : bitDepth;

    // Prep to read
    streampos pos = file.tellg();
    file.sync();
    file.seekg(pos, ios::beg);

    while (bitsRemaining > 0) {
        // Read byte with encoded data
        if (BYTES_TO_SKIP > 0) {
            file.seekg(BYTES_TO_SKIP, ios::cur);
        }

        // Prepare to read
        file.read((char*)&buffer, sizeof(char));
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
    int depth = bitDepth;
    if (depth > CHAR_BIT) {
        depth = CHAR_BIT;
    }
    // Loop until all bits in the character have been embedded.
    char data;
    size_t bitsPut = 0, bitPos;

    // Prep to write
    while (bitsPut < CHAR_BIT) {
        // Skip bytes in the file to find the low order bit if necessary
        if (BYTES_TO_SKIP > 0) {
            file.seekg(BYTES_TO_SKIP, ios::cur);
        }
        // Read the byte containing low order bit(s)
        file.read(&data, 1);
        for (int bitPos = CHAR_BIT; bitPos - depth > -1 && bitsPut < CHAR_BIT; bitPos -= depth, byte >>= 1) {
            // Shift left to get a zero bit in the correct bit position
            char clearBitMask = 0xFE << (bitPos - depth);
            // Create a mask for pos to fill one's where zeros got filled
            unsigned char correctionMask = 0xFF >> (CHAR_BIT - bitPos + depth);
            // Fill with ones where zeros got filled, except for the bit position
            clearBitMask |= correctionMask;
            // Move the low order bit of the byte to the bit position
            char mask = (byte & 1) << (bitPos - depth);
            // Clear the bit in data, and then replace it with the low order bit
            data = (data & clearBitMask) | mask;
            ++bitsPut;
        }
        // Replace the byte in the file with the bit embedded byte
        buffer.push_back(data);
    }
}

bool StegImage::messageFits(string) {
    return (s.length() * CHAR_BIT) <= (width*height*(hasAlpha ? 4 : 3));
}

void StegImage::flushAndClose() {
    // Create write handle
    file.close();
    file.clear();
    file.open(filename, ios::ate | ios::in | ios::out | ios::binary);
    file.seekp(start, ios::beg);
    assert(!file == false);

    // Write data
    for (char c : buffer)
        file.write(&c, 1);
    file.flush();
    file.close();
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
    if (in) {
        // Seek put head to the correct position, if needed
        in.seekp(in.tellg(), ios::beg);
        return true;
    } else
        return false;
}
>>>>>>> parent of f9c8255... fixed message fits
