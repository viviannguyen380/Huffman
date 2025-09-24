#include <iostream>
#include "Huffman.h"

using namespace std;

int main() {
    Huffman *compressor = new Huffman();
    int choice = 0;
    string in_file, out_file;

    cout << "Would you like to:" << endl
         << "1) Compress a file" << endl
         << "2) Decompress a file" << endl;

    cin >> choice;
    if (1 == choice) {
        compressor->compress();
    }

    else if (2 == choice) {
        compressor->decode();

    } else {
        cout << "That is not a valid choice." << endl;
    }

    return 0;

}