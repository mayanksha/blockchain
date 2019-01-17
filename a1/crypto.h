#ifndef _CRYPTO_H_DEFINED_
#define _CRYPTO_H_DEFINED_

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>

#include <functional>
#include <unordered_set>
#include <boost/functional/hash.hpp>

typedef std::vector<uint8_t> uint8_vector_t;

class hash_result_t {
    // hold the bytes.
    uint8_t bytes[SHA256_DIGEST_LENGTH];

public:
    static const int SIZE;

    // constructor with empty hash.
    hash_result_t();
    // constructor with known hash.
    hash_result_t(uint8_t bytesIn[SHA256_DIGEST_LENGTH]);
    // copy constructor.
    hash_result_t(const hash_result_t& other);

    // set hash from data.
    void set_hash_from_data(const uint8_vector_t& data);
    void set_hash_from_data(const uint8_t* data, size_t sz);

    // set hash from another.
    hash_result_t& operator=(const hash_result_t& other) {
        std::copy(other.bytes, other.bytes + size(), bytes);
        return *this;
    }
    // compare equality of two hashes.
    bool operator==(const hash_result_t& other) const;
    // compare inequality of two hashes.
    bool operator!=(const hash_result_t& other) const {
        return !(*this == other);
    }
    // order two hashes.
    bool operator<(const hash_result_t& other) const;
    // access bytes.
    uint8_t& operator[](unsigned i) {
        assert(i < size());
        return bytes[i];
    }
    // const accessor for bytes.
    uint8_t operator[](unsigned i) const {
        assert(i < size());
        return bytes[i];
    }

    // accessor methods.
    const uint8_t* data() const { return bytes; }
    uint8_t* data() { return bytes; }
    size_t size() const { return SIZE; }

};

std::ostream& operator<<(std::ostream& out, const hash_result_t& hash);

namespace std {
    template<> struct hash<hash_result_t>
    {
        typedef hash_result_t argument_type;
        typedef size_t result_type;

        result_type operator()(argument_type const& s) const noexcept
        {
            std::size_t seed = 0;
            for (unsigned i=0; i != SHA256_DIGEST_LENGTH; i++) {
                boost::hash_combine(seed, s[i]);
            }
            return seed;
        }
    };
}

struct rsa_private_key_t {
    RSA* rsa;
    EVP_PKEY* privateKey;

    // constructor from private key in PEM format.
    rsa_private_key_t(const std::string& text);
    // constructor from private key in DER format.
    rsa_private_key_t(const uint8_t* der, size_t derSz);
    // construct from RSA object.
    rsa_private_key_t(RSA* rsa_in);
    // destructor.
    ~rsa_private_key_t();
    // sign a message.
    void sign(const uint8_t* blk, size_t sz, 
              uint8_vector_t& sign) const;

    // create from PEM file.
    static rsa_private_key_t* create_from_PEM(const char* filename);
    // create from DER file.
    static rsa_private_key_t* create_from_DER(const char* filename);
};


struct rsa_public_key_t {
    RSA* rsa;
    EVP_PKEY* publicKey;
    uint8_vector_t keyDER;

    // construct from public key in PEM format.
    rsa_public_key_t(const std::string& text);
    // construct from public key in DER format.
    rsa_public_key_t(const uint8_t* der, size_t derSz);
    // destructor.
    ~rsa_public_key_t();
    // verify a message.
    bool verify(const uint8_t* blk, size_t blkSz, 
                const uint8_t* sign, size_t signSz) const;

    // helper.
    void _createDERBuffer();

    // create from PEM file.
    static rsa_public_key_t* create_from_PEM(const char* filename);
    // create from DER file.
    static rsa_public_key_t* create_from_DER(const char* filename);
};

typedef std::pair<rsa_private_key_t*, rsa_public_key_t*> keypair_t;
keypair_t generate_rsa_keys(int bits = 2048, unsigned e = RSA_F4);

rsa_public_key_t* create_public_key(const char* filename);

void crypto_init();
void crypto_finalize();

void sha256(const uint8_t* data, size_t sz, 
            unsigned char hash[SHA256_DIGEST_LENGTH]);

std::shared_ptr< uint8_vector_t > readBinaryBytes(const char* filename);
std::string readText(const char* filename);
#endif // _CRYPTO_H_DEFINED_
