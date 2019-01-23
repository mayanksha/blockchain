#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "crypto.h"
#include "blockchain.h"
#include "transaction.h"

txn_t::txn_t()
{
}

txn_t::txn_t(std::istream& in)
{
    read(in);
}

txn_t::txn_t(const uint8_vector_t& pubKey, const hash_result_t& src_addr,
             const hash_result_t& dst_addr, const hash_result_t& chng_addr,
             uint64_t amt,
             const hash_result_t& hash, const uint8_vector_t& sign)
    : public_key(pubKey)
    , source_addr(src_addr)
    , dest_addr(dst_addr)
    , change_addr(chng_addr)
    , amount(amt)
    , tx_hash(hash)
    , tx_sign(sign)
    , valid(false)
{
}

txn_t::txn_t(
    const rsa_private_key_t& privkey, const uint8_vector_t& pubkey,
    const hash_result_t& src_addr, const hash_result_t& dst_addr,
    const hash_result_t& chng_addr, uint64_t amt)
    : public_key(pubkey)
    , source_addr(src_addr)
    , dest_addr(dst_addr)
    , change_addr(chng_addr)
    , amount(amt)
    , valid(false)
{
    // compute hash.
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, public_key.data(), public_key.size());
    SHA256_Update(&sha256, source_addr.data(), source_addr.size());
    SHA256_Update(&sha256, dest_addr.data(), dest_addr.size());
    SHA256_Update(&sha256, change_addr.data(), change_addr.size());
    SHA256_Update(&sha256, &amount, sizeof(amount));
    SHA256_Final(const_cast<uint8_t*>(tx_hash.data()), &sha256);

    // sign the transaction.
    privkey.sign(tx_hash.data(), tx_hash.size(), tx_sign);
}

bool txn_t::validate() 
{
#if DEBUG
	std::cout << "[T_validate] Inside!\n";
#endif
	uint8_t pubKeyHash[SHA256_DIGEST_LENGTH];
	uint8_t tx_hash_verify[SHA256_DIGEST_LENGTH];
	
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, this->public_key.data(), this->public_key.size());
	SHA256_Final(pubKeyHash, &sha256);

	if (source_addr != pubKeyHash){
#if DEBUG
		std::cout << "[T_validate] source_addr != pubKeyHash" << std::endl;
#endif
		return false;
	}
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, public_key.data(), public_key.size());
	SHA256_Update(&sha256, source_addr.data(), source_addr.size());
	SHA256_Update(&sha256, dest_addr.data(), dest_addr.size());
	SHA256_Update(&sha256, change_addr.data(), change_addr.size());
	SHA256_Update(&sha256, &amount, sizeof(amount));
	SHA256_Final(tx_hash_verify, &sha256);

	if (this->tx_hash != tx_hash_verify){
#if DEBUG
		std::cout << "[T_validate] this->tx_hash != tx_hash_verify" << std::endl;
#endif
		return false;
	}

	rsa_public_key_t pubkey(this->public_key.data(), this->public_key.size());
	bool success = pubkey.verify((const uint8_t*)this->tx_hash.data(), this->tx_hash.size(), this->tx_sign.data(), this->tx_sign.size());
	if (success == false){
#if DEBUG
		std::cout << "[T_validate] Invalid signature!" << std::endl;
#endif
	}
	return success;
}

bool txn_t::balance_available(const balance_map_t& balances) const
{
    auto pos = balances.find(source_addr);
    if (pos == balances.end()) {
        return false;
    } else {
        return pos->second >= amount;
    }
}

void txn_t::update_balances(balance_map_t& balances) const
{
    auto pos = balances.find(source_addr);
    if (pos == balances.end()){
#if DEBUG
			printf("[%s], pos == balances.end() FAILED\n", __func__);
#endif
			return;
		}
    /*assert (pos != balances.end());*/
    if (pos->second < amount)
		{
#if DEBUG
			printf("[%s], (pos->second < amount) FAILED\n", __func__);
#endif
			return;
		}
    /*assert (pos->second >= amount);*/

    // calculate change.
    uint64_t change = pos->second - amount;
		if (balances.find(dest_addr) == balances.end()){
			balances[dest_addr] = amount;
		}
		else 
			balances[dest_addr] += amount;
		if (balances.find(change_addr) == balances.end()){
			balances[change_addr] = change;
		}
		else 
			balances[dest_addr] += change;

		// Remove the source address from the balance_map
		balances.erase(pos);
    // source has no more balance.
    /*balances[source_addr] = 0;*/
}

size_t txn_t::size() const
{
    return sizeof(int) + sizeof(int) + sizeof(amount) +
           public_key.size() +
           source_addr.size() + dest_addr.size() + change_addr.size() +
           tx_hash.size() + tx_sign.size();
}

bool txn_t::write(std::ostream& out) const
{
    // compute size of public key.
    int pubkey_sz = public_key.size();
    assert (pubkey_sz > 0 && pubkey_sz < 1024);
    // compute size of transaction signature.
    int sign_sz = tx_sign.size();
    assert (sign_sz > 0 && sign_sz < 1024);

    // write sizes first.
    out.write((const char*) &pubkey_sz,         sizeof(pubkey_sz));
    out.write((const char*) &sign_sz,           sizeof(sign_sz));
    out.write((const char*) &amount,            sizeof(amount));
    // and now write the data.
    out.write((const char*) public_key.data(),  public_key.size());
    out.write((const char*) source_addr.data(), source_addr.size());
    out.write((const char*) dest_addr.data(),   dest_addr.size());
    out.write((const char*) change_addr.data(), change_addr.size());
    out.write((const char*) tx_hash.data(),     tx_hash.size());
    out.write((const char*) tx_sign.data(),     tx_sign.size());
    return !out.bad();
}

bool txn_t::read(std::istream& in)
{
    int pubkey_sz, sign_sz;

    // read sizes first.
    in.read((char*) &pubkey_sz, sizeof(pubkey_sz));
    in.read((char*) &sign_sz,   sizeof(sign_sz));
    in.read((char*) &amount,    sizeof(amount));

    // TODO: must remove these checks and have students reinsert them.
    // make sure sizes are sane.
    if (pubkey_sz < 0 || pubkey_sz >= 1024) return false;
    if (sign_sz < 0 || sign_sz > 1024) return false;

    // this block is set to be invalid.
    valid = false;

    // set sizes.
    public_key.resize(pubkey_sz);
    tx_sign.resize(sign_sz);

    // read the data.
    in.read((char*) public_key.data(),  public_key.size());
    in.read((char*) source_addr.data(), source_addr.size());
    in.read((char*) dest_addr.data(),   dest_addr.size());
    in.read((char*) change_addr.data(), change_addr.size());
    in.read((char*) tx_hash.data(),     tx_hash.size());
    in.read((char*) tx_sign.data(),     tx_sign.size());

    return !in.bad();
}

size_t txn_t::read(uint8_t* buffer, size_t bufsz)
{
    int pubkey_sz, sign_sz;
    uint8_t* bufferStart = buffer;

    const size_t TX_HDR_SZ = 
        sizeof(pubkey_sz) + sizeof(sign_sz) + sizeof(amount);

    if (bufsz < TX_HDR_SZ) {
        return 0;
    }

    pubkey_sz = *((int*) buffer); buffer += sizeof(pubkey_sz);
    sign_sz = *((int*) buffer); buffer += sizeof(sign_sz);
    amount = *((uint64_t*) buffer); buffer += sizeof(amount);

    // TODO: must remove these checks and have students reinsert them.
    if (pubkey_sz < 0 || pubkey_sz >= 1024) return 0;
    if (sign_sz < 0 || sign_sz > 1024) return 0;

    // this block is set to be invalid.
    valid = false;

    public_key.resize(pubkey_sz);
    tx_sign.resize(sign_sz);

    const size_t TX_BODY_SZ = 
        public_key.size() +
        source_addr.size() + dest_addr.size() + change_addr.size() +
        tx_hash.size() + tx_sign.size();

    const size_t TX_SZ = TX_HDR_SZ + TX_BODY_SZ;
    if (bufsz < TX_SZ) {
        return 0;
    }

    // read the public key.
    std::copy(buffer, buffer + public_key.size(), public_key.data());
    buffer += public_key.size();
    // source addr.
    std::copy(buffer, buffer + source_addr.size(), source_addr.data());
    buffer += source_addr.size();
    // dest addr.
    std::copy(buffer, buffer + dest_addr.size(), dest_addr.data());
    buffer += dest_addr.size();
    // change addr.
    std::copy(buffer, buffer + change_addr.size(), change_addr.data());
    buffer += change_addr.size();
    // tx hash
    std::copy(buffer, buffer + tx_hash.size(), tx_hash.data());
    buffer += tx_hash.size();
    // tx sign
    std::copy(buffer, buffer + tx_sign.size(), tx_sign.data());
    buffer += tx_sign.size();

    size_t readSz = (buffer - bufferStart);
    assert (readSz == TX_SZ);
    return TX_SZ;
}

std::ostream& operator<<(std::ostream& out, const txn_t& txn)
{
    out << "sender  : " << txn.source_addr << std::endl;
    out << "receiver: " << txn.dest_addr   << std::endl;
    out << "change  : " << txn.change_addr << std::endl;
    out << "amount  : " << txn.amount      << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const balance_map_t& bmap)
{
    for (auto p : bmap) {
        out << p.first << ": " << p.second << std::endl;
    }
    return out;
}

