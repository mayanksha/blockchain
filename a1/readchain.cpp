#include <iomanip>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "crypto.h"
#include "blockchain.h"
#include "transaction.h"

int main(int argc, char* argv[])
{
    using namespace std;

    crypto_init();
    if (argc != 2) {
        cerr << "Syntax error. Usage: " << endl;
        cout << "    readchain <filename>" << endl;
        return 1;
    }

    block_map_t bmap;
    // open file.
    ifstream ifs(argv[1], ios::in | ios::binary | ios::ate);
    if (!ifs) {
        cerr << "Unable to open file: " << argv[1] << endl;
        return 1;
    }
    // find file size.
    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);
    // slurp file.
    vector<uint8_t> bytes(fileSize);
    ifs.read((char*)bytes.data(), fileSize);
    if (!ifs) {
        cerr << "Unable to read data from file." << endl;
        return 2;
    }
    // close file.
    ifs.close();

    uint8_t* data = bytes.data();
    size_t bytesLeft = fileSize;
    shared_ptr<block_t> tip;
    for(int i = 0; bytesLeft > 0; i++) {
        shared_ptr<block_t> blk(new block_t());
        size_t bytesParsed = blk->read(data, bytesLeft, bmap);
        if(bytesParsed > 0) {
            if (blk->validate()) {
                bmap[blk->blk_hash] = blk;
                if (!tip) {
                    tip = blk;
                } else if(tip->length < blk->length) {
                    tip = blk;
                }
            }
            // if block was invalid we ignore it.
            // update our pointers
            data += bytesParsed;
            bytesLeft -= bytesParsed;
        } else {
            break;
        }
    }
    if (tip) {
        cout << "blk: " << tip->blk_hash << "; len: " << tip->length << endl;
    } else {
        cout << "invalid." << std::endl;
    }

    crypto_finalize();
    return 0;
}
