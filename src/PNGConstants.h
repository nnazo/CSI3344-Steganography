/*
 * Author: Jacob Curtis, Joseph Perez, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 4/8/2020
 */

#ifndef PNG_CONSTANTS_H
#define PNG_CONSTANTS_H

// Identifies the PPM file type we need
#define MAGIC_NUMBER_STRING_LOWER "p6"
#define MAGIC_NUMBER_STRING_UPPER "P6"

// Identifies the PNG header
#define HEADER "IHDR"

// Identifies the PNG data
#define DATA "IDAT"

// The value at which it is assumed dimensions are in little endian.
#define TIPPING_POINT 0x0100

// PNG code for a palette-based PNG
#define PALETTE 3

// Width in bytes of the dimension fields in a PNG
#define DIM_WIDTH 4

// Width in bytes of the type and bit depth fields
#define DEPTH_TYPE_WIDTH 1

// Represents the bit containing the presence of an alpha field
#define ALPHA_DETECT_MASK 0x04

#endif // PNG_CONSTANTS_H