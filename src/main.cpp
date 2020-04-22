/*
 * Author: Jacob Curtis, Joseph Perez, Josh Huertas, Joseph Yu, Micah Schiewe
 * Assignment Title: Image Steganography
 * Assignment Description: This file contains the code for a simple
 *                         steganographic message embed command-line tool.
 * Due Date: INSERT_WHEN_KNOWN
 * Date Created: 3/26/2020
 * Date Last Modified: 3/26/2020
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include "StegImage.h"

using namespace std;

enum Mode { EMBED, DECODE };
struct Input {
    const Mode mode;
    string image;
    string out;
    string msg;
};

void verifyExtension(const string &file, const string &extension, int mode, int arg);
Input getInput(int argc, char **argv);
void readMessage(StegImage &image, const string &file);
void writeMessage(StegImage &image, const string &msgFile);

int main(int argc, char **argv) {
    Input in = getInput(argc, argv);

    if (in.mode == EMBED) {
        ifstream messageFile(in.msg);
        if (!messageFile) {
            cerr << "Error: Could not open " << in.msg << endl;
            exit(EXIT_FAILURE);
        }
        string message;
        getline(messageFile, message, '\0');
        StegImage image(in.image);
        writeMessage(image, message);
    } else if (in.mode == DECODE) {
        StegImage image(in.image);
        readMessage(image, in.msg);
    }

    return 0;
}

void verifyExtension(const string &file, const string &extension, int mode, int arg) {
    if (file.find(extension) != file.size() - extension.size()) {
        cerr << "usage: must provide a " << extension << " file for ";
        switch (arg) {
            case 1: cerr << "the input image file"; break;
            case 2: {
                if (mode == EMBED) {
                    cerr << "the output image file";
                } else {
                    cerr << "the output message file";
                }
                break;
            }
            case 3: cerr << "the input message file"; break;
        }
        cerr << endl;
        exit(EXIT_FAILURE);
    }
}

Input getInput(int argc, char **argv) {
    // Check number of arguments
    if (argc < 4 || argc > 5) {
        cerr << R"(usage: ./CSI3344_Steganography -e "img.pbm" "steg.pbm" "message.txt")" << endl;
        cerr << R"(       ./CSI3344_Steganography -d "steg.pbm" "message.txt")" << endl;
        exit(EXIT_FAILURE);
    }

    Mode mode;
    string flag, img, out, msg;
    flag = argv[1];
    img = argv[2];
    if (argc == 5 && flag == "-e") {
        out = argv[3];
        msg = argv[4];
        mode = EMBED;
    } else if (argc == 4 && flag == "-d") {
        msg = argv[3];
        mode = DECODE;
    } else {
        cerr << "usage: must provide -e or -d flag" << endl;
        exit(EXIT_FAILURE);
    }

    // Verify input PBM extension
    verifyExtension(img, ".pbm", 1, mode);
    if (mode == EMBED) {
        verifyExtension(out, ".pbm", 2, mode);
        verifyExtension(msg, ".txt", 3, mode);
    } else {
        verifyExtension(msg, ".txt", 2, mode);
    }
    return Input {
        mode,
        img,
        out,
        msg,
    };
}

void readMessage(StegImage &image, const string &file) {
    ofstream messageFile(file);
    unsigned int length = 0;

    for (int i = 0; i < 4; ++i) {
        unsigned int byte = image.get() << (8 * i);
        length |= byte;
    }

    for (unsigned int i = 0; i < length; ++i) {
        char ch = image.get();
        messageFile << ch;
        cout << ch;
    }

    messageFile.close();
}

void writeMessage(StegImage &image, const string &msgFile) {
    ifstream file(msgFile);
    if (!file) {
        cerr << "Error: Could not open " << msgFile << endl;
        exit(EXIT_FAILURE);
    }
    file.seekg(0, ios::end);
    unsigned int size = file.tellg();

    for (int i = 0; i < 4; ++i) {
        unsigned char byte = 0x000000FF & size;
        image.put(byte);
        size >>= 8;
    }

    while (file) {
        image.put(file.get());
    }

    image.flushAndClose(/*in.out*/);
}
