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
#include <vector>

#include "PNGConstants.h"
#include "StegImage.h"

using namespace std;

#define FAIL() exit(EXIT_FAILURE)
#define BAD_MSG "Provided file is corrupted or invalid"
#define badOpen() { cerr << BAD_MSG << endl; FAIL(); }

enum Mode { EMBED, DECODE };
struct Input {
    const Mode mode;
    string image;
    string out;
    string msg;
};

void verifyExtension(const string& file, const vector<string>& extensions, int mode, int arg);
Input getInput(int argc, char **argv);
void readMessage(StegImage &image, const string &file);
void writeMessage(StegImage &image, const string &msgFile, const string &out);

int main(int argc, char **argv) {
    Input in = getInput(argc, argv);

    if (in.mode == EMBED) {
        StegImage image(in.image);
        if (!image.isInError())
            writeMessage(image, in.msg, in.out);
        else
            badOpen();
    } else if (in.mode == DECODE) {
        StegImage image(in.image);
        if (!image.isInError())
            readMessage(image, in.msg);
        else
            badOpen();
    }

    return 0;
}

void verifyExtension(const string &file, const vector<string>& extensions, int mode, int arg) {
    bool valid = false;

    // Search extensions list for a valid extension
    for (auto i = extensions.begin(); i != extensions.end() && !valid; i++)
        valid = file.find(*i) == file.size() - i->size();

    if (!valid) {
        // Tell user valid file formats
        cerr << "usage: must provide a " << extensions[0];
        auto i = extensions.begin();
        i++;
        for (; i != extensions.end(); i++)
            cerr << " or " << (*i);
        cerr << " file for ";

        // Alert the user which file was invalid
        switch (arg) {
            case 2: cerr << "the input image file"; break;
            case 3: {
                if (mode == EMBED) {
                    cerr << "the output image file";
                } else {
                    cerr << "the output message file";
                }
                break;
            }
            case 4: cerr << "the input message file"; break;
            default: cerr << "unspecified argument";
        }
        cerr << endl;
        FAIL();
    }
}

Input getInput(int argc, char **argv) {
    // Check number of arguments
    if (argc < 4 || argc > 5) {
        cerr << R"(usage: ./CSI3344_Steganography -e "img.pbm" "steg.pbm" "message.txt")" << endl;
        cerr << R"(       ./CSI3344_Steganography -d "steg.pbm" "message.txt")" << endl;
        FAIL();
    }

    Mode mode;
    string flag, img, out, msg;
    const vector<string> ACCEPTED_IMG_FORMATS = { ".ppm", ".pbm" };
    const vector<string> ACCEPTED_MSG_FORMATS = { ".txt", ".rtf" };

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
        FAIL();
    }

    // Verify input extensions indicate valid files
    verifyExtension(img, ACCEPTED_IMG_FORMATS, 2, mode);
    if (mode == EMBED) {
        verifyExtension(out, ACCEPTED_IMG_FORMATS, 3, mode);
        verifyExtension(msg, ACCEPTED_MSG_FORMATS, 4, mode);
    } else {
        verifyExtension(msg, ACCEPTED_MSG_FORMATS, 3, mode);
    }
    return Input {
        mode,
        img,
        out,
        msg,
    };
}

void readMessage(StegImage &image, const string &file) {
    ofstream messageFile(file, ios::out | ios::binary);
    unsigned int length = 0;

    // Read in length of message
    for (int i = 0; i < 4; ++i) {
        unsigned int byte = image.get() << (8 * i);
        length |= byte;
    }

    // Read in message
    char ch;
    for (unsigned int i = 0; i < length; ++i) {
        ch = image.get();
        messageFile << ch;
        cout << ch;
    }

    messageFile.close();
}

void writeMessage(StegImage &image, const string &msgFile, const string &out) {
    ifstream file(msgFile, ios::binary);

    // Ensure valid file
    if (!file) {
        cerr << "Error: Could not open " << msgFile << endl;
        FAIL();
    }

    // Prep for operation
    file.seekg(0, ios::end);
    unsigned int size = file.tellg();
    unsigned char byte;

    // Ensure that the message is not too long
    if (!image.messageFits(size + 4)) {
        cerr << "Error: Message does not fit in file";
        FAIL();
    }

    // Write the size of the message
    for (int i = 0; i < 4; ++i) {
        byte = 0x000000FF & size;
        image.put(byte);
        size >>= 8;
    }

    // Write the message's characters
    file.seekg(0, ios::beg);
    while (file) {
        file.read((char*) &byte, 1);
        image.put(byte);
    }

    // End write
    file.close();
    image.flushAndClose(out);
    cout << "Message embed successful" << endl;
}
