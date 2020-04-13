/*
 * Author: Jacob Curtis, Joseph Perex, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 3/26/2020
 */

#include <iostream>
#include <cstdlib>
#include "StegImage.h"

using namespace std;

enum Mode { EMBED, DECODE };
struct Input {
    const Mode mode;
    const string image;
    const string out;
};

void verifyPngExtension(string file);
Input getInput(int argc, char **argv);
string readMessage(const StegImage &image, const string &file);
void writeMessage(const StegImage &image, const string &message);

int main(int argc, char **argv) {
    Input in = getInput(argc, argv);
    if (in.mode == EMBED) {
        // TODO
    } else if (in.mode == DECODE) {
        // TODO
    }
    return 0;
}

void verifyPngExtension(string file) {
    if (file.find(".png") != file.size() - 4) {
        cerr << "usage: must provide a PNG image to embed a message" << endl;
        exit(EXIT_FAILURE);
    }
}

Input getInput(int argc, char **argv) {
    // Check number of arguments
    if (argc != 4) {
        cerr << R"(usage: ./CSI3344_Steganography -e "img.png" "message")" << endl;
        cerr << R"(       ./CSI3344_Steganography -d "steg.png" "message.txt")" << endl;
        exit(EXIT_FAILURE);
    }

    Mode mode;
    string flag = argv[1];
    string img = argv[2];
    string out = argv[3];

    // Verify input PNG filename
    verifyPngExtension(img);

    // Check flag mode
    if (flag == "-e") {
        mode = EMBED;
    } else if (flag == "-d") {
        mode = DECODE;
        verifyPngExtension(out);
    } else {
        cerr << "usage: must provide -e or -d flag" << endl;
        exit(EXIT_FAILURE);
    }

    return Input {
        mode,
        img,
        out,
    };
}

string readMessage(const StegImage &image, const string &file) {
    // TODO
    return "";
}

void writeMessage(const StegImage &image, const string &message) {
    // TODO
}
