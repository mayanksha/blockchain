#include "crypto.h"
#include <memory>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

const int hash_result_t::SIZE = SHA256_DIGEST_LENGTH;

// construct empty hash.
hash_result_t::hash_result_t()
{
    std::fill(bytes, bytes + SIZE, 0);
}
// create from bytes that hold hash.
hash_result_t::hash_result_t(uint8_t bytesIn[SIZE])
{
    std::copy(bytesIn, bytesIn + SIZE, bytes);
}
// copy constructor.
hash_result_t::hash_result_t(const hash_result_t& other)
{
    std::copy(other.bytes, other.bytes + SIZE, bytes);
}
// hash data and store result.
void hash_result_t::set_hash_from_data(const uint8_t* data, size_t sz)
{
    sha256(data, sz, bytes);
}

void hash_result_t::set_hash_from_data(const uint8_vector_t& data)
{
    sha256(data.data(), data.size(), bytes);
}

// compare equality two hashes.
bool hash_result_t::operator==(const hash_result_t& other) const
{
    for(unsigned i=0; i != SIZE; i++) {
        if (bytes[i] != other.bytes[i]) {
            return false;
        }
    }
    return true;
}

// ordering of hashes.
bool hash_result_t::operator<(const hash_result_t& other) const
{
    for(unsigned i=0; i != SIZE; i++) {
        if (bytes[i] < other.bytes[i]) {
            return true;
        } else if(bytes[i] > other.bytes[i]) {
            return false;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const hash_result_t& hash)
{
    using namespace std;

    ios_base::fmtflags f(out.flags());
    char c = out.fill();
    out << hex << setw(2);
    for(unsigned i = 0; i != hash.size(); i++) {
        out << setw(2) << setfill('0') << (int) hash[i];
    }
    out << setfill(c);
    out.flags(f);
    return out;
}


// construct from PEM.
rsa_private_key_t::rsa_private_key_t(const std::string& text)
    : rsa(NULL)
    , privateKey(NULL)
{
    // create a BIO object.
    BIO *keybio = BIO_new_mem_buf((void*) text.c_str(), -1);
    assert (keybio);

    // read the key.
    rsa = PEM_read_bio_RSAPrivateKey(keybio, NULL, NULL, NULL);
    if (rsa == NULL) { throw std::invalid_argument("Invalid PEM encoded key."); }

    // free the BIO.
    BIO_free(keybio);

    // convert create EVP PKEY
    privateKey = EVP_PKEY_new();
    assert (privateKey != NULL);
    EVP_PKEY_assign_RSA(privateKey, rsa);
}

// construct from DER.
rsa_private_key_t::rsa_private_key_t(const uint8_t* der, size_t derSz)
    : rsa(NULL)
    , privateKey(NULL)
{
    // convert to RSA object.
    rsa = d2i_RSAPrivateKey(NULL, &der, derSz);
    if (rsa == NULL) { throw std::invalid_argument("Invalid DER file."); }

    // convert create EVP PKEY
    privateKey = EVP_PKEY_new();
    assert (privateKey != NULL);
    EVP_PKEY_assign_RSA(privateKey, rsa);
}

// construct from RSA object.
rsa_private_key_t::rsa_private_key_t(RSA* rsa_in)
    : rsa(rsa_in)
    , privateKey(NULL)
{
    // convert to RSA object.
    if (rsa == NULL) { throw std::invalid_argument("Invalid DER file."); }

    // convert create EVP PKEY
    privateKey = EVP_PKEY_new();
    assert (privateKey != NULL);
    EVP_PKEY_assign_RSA(privateKey, rsa);
}

rsa_private_key_t::~rsa_private_key_t()
{
    // free the private key.
    EVP_PKEY_free(privateKey);
}


void rsa_private_key_t::sign(
        const uint8_t* blk, size_t sz, uint8_vector_t& sign) const
{
    EVP_MD_CTX* rsaCtx = EVP_MD_CTX_create();

    // sign.
    int r = EVP_DigestSignInit(rsaCtx, NULL, EVP_sha256(), NULL, privateKey);
    assert (r > 0);

    // update.
    r = EVP_DigestSignUpdate(rsaCtx, blk, sz);
    assert (r > 0);
    
    // get length.
    size_t signLen;
    r = EVP_DigestSignFinal(rsaCtx, NULL, &signLen);
    assert (r > 0);

    // allocate memory
    sign.resize(signLen);
    r = EVP_DigestSignFinal(rsaCtx, sign.data(), &signLen);
    assert (r > 0);

    // cleanup.
    EVP_MD_CTX_cleanup(rsaCtx);
    EVP_MD_CTX_destroy(rsaCtx);
}

rsa_private_key_t* rsa_private_key_t::create_from_PEM(const char* filename)
{
    std::string privateKey = readText(filename);
    if (privateKey.length() > 0) {
        return new rsa_private_key_t(privateKey);
    } else {
        return NULL;
    }
}

rsa_private_key_t* rsa_private_key_t::create_from_DER(const char* filename)
{
    auto privateKey = readBinaryBytes(filename);
    if (privateKey->size() > 0) {
        return new rsa_private_key_t(privateKey->data(), privateKey->size());
    } else {
        return NULL;
    }
}

rsa_public_key_t::rsa_public_key_t(const std::string& text)
    : rsa(NULL)
    , publicKey(NULL)
{
    // create a BIO object.
    BIO *keybio = BIO_new_mem_buf((void*) text.c_str(), -1);
    assert (keybio != NULL);

    // read the key.
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    if (rsa == NULL) { throw std::invalid_argument("Invalid PEM file."); }

    // free the BIO.
    BIO_free(keybio);

    // convert create EVP PKEY
    publicKey = EVP_PKEY_new();
    assert (publicKey != NULL);
    EVP_PKEY_assign_RSA(publicKey, rsa);

    // create the DER encoding.
    _createDERBuffer();
}

// construct from DER.
rsa_public_key_t::rsa_public_key_t(const uint8_t* der, size_t derSz)
    : rsa(NULL)
    , publicKey(NULL)
{
    // convert to RSA object.
    rsa = d2i_RSA_PUBKEY(NULL, &der, derSz);
    if (rsa == NULL) { throw std::invalid_argument("Invalid DER encoded key."); }

    // create EVP PKEY
    publicKey = EVP_PKEY_new();
    assert (publicKey != NULL);
    EVP_PKEY_assign_RSA(publicKey, rsa);

    // create the DER encoding.
    _createDERBuffer();
}

rsa_public_key_t::~rsa_public_key_t()
{
    EVP_PKEY_free(publicKey);
}

void rsa_public_key_t::_createDERBuffer()
{
    // create the DER data.
    uint8_t* derBuffer = NULL;
    auto derBufferLen = i2d_RSA_PUBKEY(rsa, &derBuffer);
    assert (derBufferLen > 0);
    keyDER.resize(derBufferLen);
    std::copy(derBuffer, derBuffer + derBufferLen, keyDER.begin());
    free(derBuffer);
}

bool rsa_public_key_t::verify(
        const uint8_t* blk, size_t blkSz, 
        const uint8_t* sign, size_t signSz) const
{
    EVP_MD_CTX* rsaCtx = EVP_MD_CTX_create();

    // hashing init.
    int r = EVP_DigestVerifyInit(rsaCtx, NULL, EVP_sha256(), NULL, publicKey);
    assert (r > 0);

    // do the hashing.
    r = EVP_DigestVerifyUpdate(rsaCtx, blk, blkSz);
    assert (r > 0);

    // check signature.
    int authStatus = EVP_DigestVerifyFinal(rsaCtx, sign, signSz);

    // cleanup.
    EVP_MD_CTX_cleanup(rsaCtx);
    EVP_MD_CTX_destroy(rsaCtx);

    // check return value.
    assert (authStatus == 1 || authStatus == 0);

    if (authStatus == 1) {
        return true;
    } else {
        return false;
    } 
    // should never get here.
    assert (false);
}

rsa_public_key_t* rsa_public_key_t::create_from_PEM(const char* filename)
{
    std::string publicKey = readText(filename);
    if (publicKey.length() > 0) {
        return new rsa_public_key_t(publicKey);
    } else {
        return NULL;
    }
}

rsa_public_key_t* rsa_public_key_t::create_from_DER(const char* filename)
{
    auto publicKey = readBinaryBytes(filename);
    if (publicKey->size() > 0) {
        return new rsa_public_key_t(publicKey->data(), publicKey->size());
    } else {
        return NULL;
    }
}

keypair_t generate_rsa_keys(int bits, unsigned e)
{
    int ret = 0;
    RSA *rsa = NULL;
    BIGNUM *bne = NULL;

    // set e
    bne=BN_new();
    ret=BN_set_word(bne,e);
    assert(ret==1);

    // generate key.
    rsa=RSA_new();
    ret=RSA_generate_key_ex(rsa,bits,bne,NULL);
    assert(ret==1);

    // get rid of the bignum.
    BN_free(bne);

    rsa_private_key_t *privkey = new rsa_private_key_t(rsa);

    // dump public key in DER format
    uint8_t* derBuffer = NULL;
    auto derBufferLen = i2d_RSA_PUBKEY(rsa, &derBuffer);
    assert (derBufferLen > 0);

    // create a public key object out of it.
    rsa_public_key_t* pubkey = new rsa_public_key_t(derBuffer, derBufferLen);
    free(derBuffer);

    return keypair_t(privkey, pubkey);
}

void sha256(const uint8_t* data, size_t sz, unsigned char hash[SHA256_DIGEST_LENGTH])
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, sz);
    SHA256_Final(hash, &sha256);
}

void crypto_init()
{
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
}


void crypto_finalize()
{
    CONF_modules_free();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    EVP_cleanup();
}

std::shared_ptr< uint8_vector_t > readBinaryBytes(const char* filename)
{
    using namespace std;

    // open file.
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    // memory.
    shared_ptr< vector<uint8_t> > bytes(new vector<uint8_t>(pos));
    uint8_t* data = bytes->data();

    // read file.
    ifs.seekg(0, ios::beg);
    ifs.read((char*)data, pos);

    // close file.
    ifs.close();

    return bytes;
}

std::string readText(const char* filename)
{
    using namespace std;

    // open file.
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    // memory.
    size_t sz = static_cast<size_t>(pos) + static_cast<size_t>(1);
    std::unique_ptr<char[]> text(new char[sz]);

    // read file.
    ifs.seekg(0, ios::beg);
    ifs.read(text.get(), pos);

    // close file.
    ifs.close();

    text[pos] = '\0';
    return std::string(text.get());
}
