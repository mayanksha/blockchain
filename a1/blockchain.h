#ifndef __BLOCKCHAIN_H_DEFINED__
#define __BLOCKCHAIN_H_DEFINED__

#include <string>
#include <memory>
#include <unordered_map>

#include "crypto.h"
#include "transaction.h"

extern const int BLOCK_REWARD;
extern const int MAX_TRANSACTIONS;

class block_t;
typedef std::unordered_map<hash_result_t, std::shared_ptr<block_t> > block_map_t;

class block_t {
private:
    // flag that determines whether a block is valid.
    bool valid;

    // balances for each address.
    balance_map_t balances;

public:
    // length of the chain
    unsigned length;

    // constructor for coinbase block.
    block_t();
    // constructor for block with a previous pointer.
    block_t(std::shared_ptr<block_t> prev_block);

    // reward address.
    hash_result_t reward_addr;

    // list of transactions.
    std::vector< std::shared_ptr<txn_t> > transactions;

    // previous block hash pointer.
    hash_result_t prev_hash;

    // block hash.
    hash_result_t blk_hash;

    // previous block pointer.
    std::shared_ptr<block_t> prev_block;

    // is this the coinbase block.
    bool is_coinbase() const;

    // validate block.
    bool validate();

    // add a transaction to the block.
    bool add_transaction(std::shared_ptr<txn_t> tx, bool check_tx);

    // update block hash.
    void set_hash();

    // update previous pointer.
    void set_prev_block(std::shared_ptr<block_t> prev_blk) {
        prev_block = prev_blk;
        if (prev_block) {
            length = prev_blk->length + 1;
        }
        reset_balances();
    }

    // size of the transaction.
    size_t size() const;

    // write a transaction to a file.
    bool write(std::ostream& out);

    // read transaction from file.
    bool read(std::istream& in, const block_map_t& bmap);

    // read transaction from memory buffer.
    size_t read(uint8_t* data, size_t bufSz, const block_map_t& bmap);

    // get balances.
    const balance_map_t& get_balances() const {
        return balances;
    }
    void set_balances(const balance_map_t& bals) {
        balances = bals;
    }
    void recompute_length();
private:
    // compute transaction hash.
    void compute_block_hash(hash_result_t& hash_result);

    // reset balances.
    void reset_balances();
};

inline size_t balance_map_size(const balance_map_t& bals) {
    return sizeof(int) +
           (bals.size() * (hash_result_t::SIZE + sizeof(uint64_t)));
}

bool write_balance_map(std::ostream& out, const balance_map_t& bals);
bool read_balance_map(std::istream& in, balance_map_t& bals);
void read_balance_map(uint8_t* data, balance_map_t& bals);

#endif
