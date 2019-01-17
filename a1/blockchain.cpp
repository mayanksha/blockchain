#include "blockchain.h"

const int BLOCK_REWARD      = 100;
const int MAX_TRANSACTIONS  = 64;

block_t::block_t()
    : valid(false)
    , length(1)
    , prev_block(NULL)
{
    reset_balances();
}

block_t::block_t(std::shared_ptr<block_t> prev)
    : valid(false)
    , length(prev->length + 1)
    , prev_hash(prev->blk_hash)
    , prev_block(prev)
{
    reset_balances();
}

bool block_t::is_coinbase() const
{
    for (unsigned i=0; i != prev_hash.size(); i++) {
        if (prev_hash[i] != 0) {
            return false;
        }
    }
    assert (prev_block == NULL);
    assert (length == 1);
    return true;
}

void block_t::compute_block_hash(hash_result_t& hash_result)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    // hash all the transactions.
    for (auto tx : transactions) {
        SHA256_Update(&sha256, tx->tx_hash.data(), tx->tx_hash.size());
    }
    // hash block reward.
    SHA256_Update(&sha256, reward_addr.data(), reward_addr.size());
    // hash previous block pointer.
    SHA256_Update(&sha256, prev_hash.data(), prev_hash.size());
    // now finalize hash.
    SHA256_Final(const_cast<uint8_t*>(hash_result.data()), &sha256);
}

void block_t::set_hash()
{
    compute_block_hash(blk_hash);
}


bool validate_block_helper(
    std::vector< std::shared_ptr<txn_t> >& transactions,
    balance_map_t& balances,
    hash_result_t& prev_hash,
    hash_result_t& reward_addr,
    hash_result_t& blk_hash,
    const uint64_t BLOCK_REWARD
);

bool block_t::validate()
{
    // create balances array.
    reset_balances();
    // TODO: replace the call to the helper with your own code.
    valid = validate_block_helper(
                transactions, balances, prev_hash, reward_addr, blk_hash, BLOCK_REWARD);
    return valid;
}

bool block_t::add_transaction(std::shared_ptr<txn_t> tx, bool check_tx)
{
    // is this a valid transaction?
    if (check_tx) {
        if (!tx->validate()) { return false; }
        if (!tx->balance_available(balances)) { return false; }
    }
    // add this to the list of txns
    transactions.push_back(tx);
    if (check_tx) {
        // update the balances.
        tx->update_balances(balances);
    }
    // return true if added.
    return true;
}

void block_t::reset_balances()
{
    if (prev_block) {
        balances = prev_block->balances;
    } else {
        balances.clear();
    }
}

size_t block_t::size() const
{
    size_t header_size =  
        sizeof(int) + reward_addr.size() +
        prev_hash.size() + blk_hash.size();
    size_t tx_sz = header_size;
    for (auto tx : transactions) {
        tx_sz += tx->size();
    }
    return tx_sz;
}

bool block_t::write(std::ostream& out)
{
    int tx_count = static_cast<int>(transactions.size());
    assert (tx_count >= 0 && tx_count <= MAX_TRANSACTIONS);
    // write # of transactions.
    out.write((const char*) &tx_count,              sizeof(tx_count));
    // write addresses.
    out.write((const char*) reward_addr.data(),     reward_addr.size());
    // write hashes.
    out.write((const char*) prev_hash.data(),       prev_hash.size());
    out.write((const char*) blk_hash.data(),        blk_hash.size());
    // now write transactions.
    for (auto tx : transactions) {
        tx->write(out);
    }
    return !out.bad();
}

bool block_t::read(std::istream& in, const block_map_t& bmap)
{
    int tx_count;
    in.read((char*) &tx_count, sizeof(tx_count));
    if (!in) {
        return false;
    }
    if (tx_count < 0 || tx_count > MAX_TRANSACTIONS) {
        return false;
    }
    valid = false;
    transactions.clear();
    length = 1;
    in.read((char*) reward_addr.data(), reward_addr.size());
    in.read((char*) prev_hash.data(), prev_hash.size());
    in.read((char*) blk_hash.data(), blk_hash.size());
    if (!in) {
        return false;
    }
    for (int i=0; i != tx_count; i++) {
        std::shared_ptr<txn_t> txn(new txn_t(in));
        if (!in) {
            return false;
        }
        transactions.push_back(txn);
    }
    // set prev_block pointer.
    if (!is_coinbase()) {
        auto pos = bmap.find(prev_hash);
        if (pos != bmap.end()) {
            set_prev_block(pos->second);
        } else {
            prev_block = NULL;
            balances.clear();
        }
    }
    return true;
}

size_t block_t::read(uint8_t* data, size_t blkSz, const block_map_t& bmap)
{
    uint8_t* dataStart = data;
    size_t blkSzStart = blkSz;

    int tx_count;
    const size_t HEADER_SZ = sizeof(tx_count) +
        reward_addr.size() + prev_hash.size() + blk_hash.size();

    if (blkSz < HEADER_SZ) return 0;

    tx_count = *((int*) data); data += sizeof(tx_count);
    transactions.clear();
    length = 1;

    // copy reward address
    std::copy(data, data+reward_addr.size(), reward_addr.data());
    data += reward_addr.size();

    // copy prev hash
    std::copy(data, data+prev_hash.size(), prev_hash.data());
    data += prev_hash.size();

    // copy blk hash
    std::copy(data, data+blk_hash.size(), blk_hash.data());
    data += blk_hash.size();

    blkSz -= HEADER_SZ;

    // read txn
    for (int i=0; i != tx_count; i++) {
        std::shared_ptr<txn_t> txn(new txn_t());
        size_t txSz = txn->read(data, blkSz);
        if(txSz == 0) {
            return 0;
        } else {
            blkSz -= txSz;
            data += txSz;
            transactions.push_back(txn);
        }
    }
    // set prev_block pointer.
    if (!is_coinbase()) {
        auto pos = bmap.find(prev_hash);
        if (pos != bmap.end()) {
            set_prev_block(pos->second);
        } else {
            prev_block = NULL;
            balances.clear();
        }
    }
    size_t s1 = data - dataStart;
    size_t s2 = blkSzStart - blkSz;
    assert (s1 == s2);
    return (s1);
}

void block_t::recompute_length()
{
    if (prev_block) {
        prev_block->recompute_length();
        length = prev_block->length + 1;
    } else {
        length = 1;
    }
}

bool write_balance_map(std::ostream& out, const balance_map_t& bals)
{
    uint32_t size = bals.size();
    out.write((char*) &size, sizeof(size));
    for (auto p : bals) {
        const hash_result_t& addr(p.first);
        uint64_t amount = p.second;
        out.write((const char*) addr.data(), addr.size());
        out.write((char*) &amount, sizeof(amount));
    }
    return out.good();
}

bool read_balance_map(std::istream& in, balance_map_t& bals)
{
    bals.clear();
    uint32_t size = 0;
    in.read((char*) &size, sizeof(size));
    if (size < 0 || in.bad()) {
        return false;
    }
    for (uint32_t i = 0; i < size; i++) {
        hash_result_t addr;
        uint64_t amt;
        in.read((char*) addr.data(), addr.size());
        in.read((char*) &amt, sizeof(amt));
        if (in.bad()) { return false; }
        bals[addr] = amt;
    }
    return true;
}

void read_balance_map(uint8_t* data, balance_map_t& bals)
{
    bals.clear();
    uint32_t size = 0;

    size = *((uint32_t*)data);
    data += sizeof(size);

    for (uint32_t i = 0; i < size; i++) {
        hash_result_t addr;
        uint64_t amt;
        std::copy(data, data + addr.size(), (uint8_t*) addr.data());
        data += addr.size();
        amt = *((uint64_t*)data);
        data += sizeof(uint64_t);
        bals[addr] = amt;
    }
}
