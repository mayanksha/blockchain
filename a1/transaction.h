#ifndef __TRANSACTION_H_DEFINED__
#define __TRANSACTION_H_DEFINED__

#include <string>
#include "crypto.h"

typedef std::map<hash_result_t, uint64_t> balance_map_t;

struct txn_t {
    // transaction details.
    uint8_vector_t public_key;
    hash_result_t source_addr;
    hash_result_t dest_addr;
    hash_result_t change_addr;
    uint64_t amount; 
    // this transaction's hash.
    hash_result_t tx_hash;
    // signature.
    uint8_vector_t tx_sign;
    // is this a valid transaction?
    bool valid;

    // read from file.
    txn_t(std::istream& in);
    // read from buffer.
    txn_t();

    // construct with parameters.
    txn_t(const uint8_vector_t& pubKey, const hash_result_t& src_addr,
          const hash_result_t& dst_addr, const hash_result_t& change_addr,
          uint64_t amount, 
          const hash_result_t& hash, const uint8_vector_t& sign);

    // construct from private key.
    txn_t(const rsa_private_key_t& privkey, const uint8_vector_t& pubkey,
          const hash_result_t& src_addr, const hash_result_t& dst_addr,
          const hash_result_t& chng_addr, uint64_t amount);

    // validate this transaction -- this just checks that
    // 1. hash   = sha256(public_key, source_addr, dst_addr, change_addr, amount)
    // 2. verify(public_key, tx_sign, tx_hash)
    bool validate();

    // check whether balances are available to perform this transaction.
    bool balance_available(const balance_map_t& balances) const;
    void update_balances(balance_map_t& balances) const;

    // get the size of the transaction.
    size_t size() const;
    // write to file.
    bool write(std::ostream& out) const;
    // read from file.
    bool read(std::istream& in);
    // read from memory buffer.
    size_t read(uint8_t* buffer, size_t bufsz);
};

// output a single transaction.
std::ostream& operator<<(std::ostream& out, const txn_t& txn);
// output the entire balance map.
std::ostream& operator<<(std::ostream& out, const balance_map_t& bmap);

#endif
