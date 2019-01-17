#include <iomanip>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "crypto.h"
#include "blockchain.h"
#include "transaction.h"

bool rewrite_chain(block_map_t& bmap, std::shared_ptr<block_t> tip, 
                   uint32_t chunks, const char* filename)
{
    assert (chunks > 1);

    // track back from the tip to the coinbase.
    std::map<block_t*, block_t*> next_map;
    next_map[tip.get()] = NULL;
    std::shared_ptr<block_t> nxt = tip;
    for(std::shared_ptr<block_t> ptr = tip->prev_block;
        ptr; ptr = ptr->prev_block) {
        next_map[ptr.get()] = nxt.get();
        nxt = ptr;
    }
    assert (next_map.size() == tip->length);
    std::cout << "next_map: " << next_map.size() << std::endl;

    // create a vector of blocks.
    std::vector<block_t*> blocks;
    blocks.reserve(tip->length);
    block_t* head = nxt.get();
    for(block_t* ptr = head; ptr != NULL; ptr = next_map[ptr]) {
        blocks.push_back(ptr);
    }
    assert (blocks.size() == tip->length);

    // compute the boundaries of the chunks
    size_t chunk_sz = blocks.size() / chunks;
    std::set<unsigned> chunkEnds;
    for(unsigned i = 0; i != chunks; i++) {
        size_t chunkEnd_i = ((i == (chunks-1)) ? blocks.size() : (chunk_sz * (i+1))) - 1;
        std::cout << "chunkEnds[" << i << "] = " << chunkEnd_i << std::endl;
        chunkEnds.insert(chunkEnd_i);
    }

    // compute the sizes of each chunk and the size of each balance map.
    std::vector<uint64_t> chunkStartOffsets;
    std::vector<uint64_t> mapStartOffsets;
    std::vector<uint64_t> mapSizes;
    uint64_t filePos = sizeof(chunks) + 3*chunks*sizeof(uint64_t);
    uint64_t prevEnd = filePos;
    for(unsigned i=0; i != blocks.size(); i++) {
        filePos += blocks[i]->size();
        if (chunkEnds.find(i) != chunkEnds.end()) {
            // mark start/end of chunk.
            chunkStartOffsets.push_back(prevEnd);
            mapStartOffsets.push_back(filePos);
            // get size of map.
            size_t mapSz = balance_map_size(blocks[i]->get_balances());
            mapSizes.push_back(mapSz);
            // setup for next chunk.
            filePos += mapSz;
            prevEnd = filePos;
        }
    }

    assert (mapStartOffsets.size() == chunks);
    assert (chunkStartOffsets.size() == chunks);
    assert (mapSizes.size() == chunks);

    // print out the chunk info
    for (unsigned i = 0; i != mapStartOffsets.size(); i++) {
        std::cout << chunkStartOffsets[i] << "/"
                  << mapStartOffsets[i]   << "/"
                  << mapSizes[i] << std::endl;
    }

    // now write this information to the file.
    std::ofstream fout(filename);

    // first the header.
    fout.write((char*) &chunks, sizeof(chunks));
    fout.write((char*) chunkStartOffsets.data(),
                sizeof(chunkStartOffsets[0])*chunkStartOffsets.size());
    fout.write((char*) mapStartOffsets.data(),
                sizeof(mapStartOffsets[0])*mapStartOffsets.size());
    fout.write((char*) mapSizes.data(),
                sizeof(mapSizes[0])*mapSizes.size());
    // and now the blocks.
    for(unsigned i=0, j = 0; i != blocks.size(); i++) {
        blocks[i]->write(fout);
        if (fout.bad()) return false;

        if (chunkEnds.find(i) != chunkEnds.end()) {
            assert (j < chunks);
            assert ((uint64_t)fout.tellp() == mapStartOffsets[j]);
            write_balance_map(fout, blocks[i]->get_balances());
            j++;
        }
    }

    if (fout.bad()) return false;
    fout.close();
    return true;
}

int main(int argc, char* argv[])
{
    using namespace std;

    crypto_init();
    if (argc != 4) {
        cerr << "Syntax error. Usage: " << endl;
        cout << "    readchain <in-filename> <num-of-chunks> <out-filename>" << endl;
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
            bmap[blk->blk_hash] = tip = blk;
            if (blk->validate()) {
                bmap[blk->blk_hash] = blk;
                cout << "[" << setw(5) << i << "] blk: " << blk->blk_hash
                     << "; #txns: "  << setw(5) << blk->transactions.size() 
                     << "; #utxo: "  << setw(5) << blk->get_balances().size()
                     << "; #blft: "  << setw(5) << bytesLeft
                     << endl;
            }

            data += bytesParsed;
            bytesLeft -= bytesParsed;
            assert (i == 0 || blk->prev_block);
        } else {
            break;
        }
    }
    if (tip) {
        std::cout << "length of longest chain: " << tip->length << std::endl;
    }

    uint32_t chunks = atoi(argv[2]);
    rewrite_chain(bmap, tip, chunks, argv[3]);

    crypto_finalize();
    return 0;
}

