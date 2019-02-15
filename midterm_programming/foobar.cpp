#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
typedef std::vector<uint8_t> uint8_vector_t;

static const char* base_58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
// Function to convert vector of bytes to its base58 representation
std::string EncodeBase58(const std::vector<unsigned char>& vect)
{
	const unsigned char* start = &(vect[0]);
	const unsigned char* end = &(vect[0]) + vect.size() * sizeof(unsigned char);
	// Allocate enough space in big-endian base58 representation.
	int size = (end - start) * 138 / 100 + 1; 
	// Skip & count leading zeroes.
	int zeroes = 0, len = 0;
	while (start != end && *start == 0) {
		start++;
		zeroes++;
	}
	std::vector<unsigned char> base_58_vect(size);
	while (start != end) {
		int carry = *start;
		int i = 0;
		// Apply "base_58_vect = base_58_vect * 256 + ch".
		for (auto it = base_58_vect.rbegin(); (carry != 0 || i < len) && (it != base_58_vect.rend()); it++, i++) {
			carry += 256 * (*it);
			*it = carry % 58;
			carry /= 58;
		}
		assert(carry == 0);
		len = i;
		start++;
	}
	// Skip leading zeroes in base58 result.
	auto it = base_58_vect.begin() - len + size;
	while (it != base_58_vect.end() && *it == 0)
		it++;
	std::string str;
	for(auto i = 0; i < (base_58_vect.end() - it); i++)
		str.push_back('0');
	while (it != base_58_vect.end()) {
		str += base_58_chars[*(it)];
		it++;
	}
	// Remove the starting zeroes from string too
	auto it2 = str.begin(); 
	while (it2 != str.end() && *it2 == '0')
		it2++;
	return str.substr(it2 - str.begin(), str.end() - it2);
}
void setup_context() {
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);
	RSA_new();
}

void cleanup_context(){
	CONF_modules_free();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
	EVP_cleanup();
}
int main() {
	// Setup the OpenSSL Context with various algorithms
	setup_context();
	int ret = 0;
	RSA *rsa = NULL;
	BIGNUM *bne = NULL;

	// set e
	bne = BN_new();
	ret = BN_set_word(bne, RSA_F4);
	assert(ret == 1);

	// generate key.
	rsa = RSA_new();
	ret = RSA_generate_key_ex(rsa, 2048 , bne , NULL);
	assert(ret == 1);
	// get rid of the bignum.
	BN_free(bne);

	// Declare a new Private Key and fill with random new value
	EVP_PKEY* privateKey = EVP_PKEY_new();
	assert (privateKey != NULL);
	// Use the declared rsa object to generate a private key
	EVP_PKEY_assign_RSA(privateKey, rsa);

	// dump public key in DER format
	uint8_t* derBuffer = NULL;
	auto derBufferLen = i2d_RSA_PUBKEY(rsa, &derBuffer);
	EVP_PKEY* publicKey = EVP_PKEY_new();
	assert (publicKey != NULL);
	// Use the rsa object to generate the corresponding public key for privateKey 
	EVP_PKEY_assign_RSA(publicKey, rsa);
	assert (derBufferLen > 0);

	// Write the un-encrypted (not password protected) private key to stdout
	PEM_write_PrivateKey(stdout, privateKey, NULL, NULL, 0, NULL, NULL);
	// Write the public key to stdout 
	PEM_write_PUBKEY(stdout, publicKey);
	unsigned char hash[SHA256_DIGEST_LENGTH];

	// Find the SHA256 Hash of the public key
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, &derBuffer[0], derBufferLen);
	SHA256_Final(hash, &sha256);

	std::vector<unsigned char> temp;
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i += 1)
		temp.push_back(hash[i]);
	std::cout << "SHA-256 Hashed and then Base58 Encoded Public Key = " << std::endl;
	// Base58 Encode the hash of public key 
	std::cout << EncodeBase58(temp) << std::endl;

	free(derBuffer);
	cleanup_context();
}















/*static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 *
 *static const char reverse_table[128] = {
 *  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
 *  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
 *  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
 *  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
 *  64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
 *  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
 *  64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
 *  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
 *};*/

/*::std::string base64_encode(uint8_vector_t &bindata)
 *{
 *  using ::std::string;
 *  using ::std::numeric_limits;
 *
 *  if (bindata.size() > (numeric_limits<string::size_type>::max() / 4u) * 3u) {
 *    throw ::std::length_error("Converting too large a string to base64.");
 *  }
 *
 *  const ::std::size_t binlen = bindata.size();
 *  // Use = signs so the end is properly padded.
 *  string retval((((binlen + 2) / 3) * 4), '=');
 *  ::std::size_t outpos = 0;
 *  int bits_collected = 0;
 *  unsigned int accumulator = 0;
 *  const uint8_vector_t::const_iterator binend = bindata.end();
 *
 *  for (uint8_vector_t::const_iterator i = bindata.begin(); i != binend; ++i) {
 *    accumulator = (accumulator << 8) | (*i & 0xffu);
 *    bits_collected += 8;
 *    while (bits_collected >= 6) {
 *      bits_collected -= 6;
 *      retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
 *    }
 *  }
 *  if (bits_collected > 0) { // Any trailing bits that are missing.
 *    assert(bits_collected < 6);
 *    accumulator <<= 6 - bits_collected;
 *    retval[outpos++] = b64_table[accumulator & 0x3fu];
 *  }
 *  assert(outpos >= (retval.size() - 2));
 *  assert(outpos <= retval.size());
 *  return retval;
 *}*/
