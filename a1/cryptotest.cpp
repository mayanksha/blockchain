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
#include <cstdlib>

#include "crypto.h"

std::string gen_random_string(size_t n)
{
    static const int range = 'z' - '0' + 1;
    std::string text;
    text.resize(n);
    for (unsigned i = 0; i != n; i++) {
        text[i] = (rand() % range) + '0';
    }
    return text;
}

std::string mutate(const std::string& text)
{
    std::string tp(text);
    unsigned pos = rand() % tp.length();
    if((rand() & 15) < 11) {
        if (islower(tp[pos])) {
            tp[pos] = toupper(tp[pos]);
        } else {
            tp[pos] = tolower(tp[pos]);
        }
    }
    return tp;
}

const int MAX_KEY_FILES = 10000;
int keys_gen = 0;

int check_signature()
{
    std::string text = gen_random_string(64);
    std::string textp = mutate(text);

    bool equal = text == textp;

    uint8_t h1[SHA256_DIGEST_LENGTH], h2[SHA256_DIGEST_LENGTH];
    // One way of doing hashing is to call the wrapper function
    // we have provided: sha256. This takes a pointer to a block
    // of data and its length and stores the hash in the final
    // argument.
    sha256((const uint8_t*) text.data(), text.length(), h1);
    // Another way is to use the LibreSSL API directly.
    SHA256_CTX sha256;
    // First we initialize the hash object.
    SHA256_Init(&sha256);
    // Then we update it using the data we want to hash.
    SHA256_Update(&sha256, textp.data(), textp.length());
    // We finalize the hash and copy the result into the
    // h2 variable.
    SHA256_Final(h2, &sha256);

    bool heq = true;
    for(unsigned i=0; i != SHA256_DIGEST_LENGTH; i++) {
        heq = heq && (h1[i] == h2[i]);
    }

    std::unique_ptr<rsa_private_key_t> priKey;
    std::unique_ptr<rsa_public_key_t> pubKey;
    // We can generate a public/private keypair using the
    // following call. But you won't be needing this in
    // your assignment.
    keypair_t p = generate_rsa_keys();
    priKey.reset(p.first);
    pubKey.reset(p.second);
    keys_gen += 1;

    std::vector<uint8_t> sign;
    // sign the data using privateKey.sign function
    priKey->sign((const uint8_t*) text.data(), text.length(), sign);
    // verify signature using the publicKey.verify function
    bool ver = pubKey->verify((const uint8_t*) textp.data(), textp.length(), sign.data(), sign.size());

    assert (equal == heq);
    assert (equal == ver);

    return ((int) equal);
}

int main()
{
    crypto_init();

    int cnt = 0;
    const int NTESTS = 100;
    for (int i = 0; i < NTESTS; i++) {
        int v = check_signature();
        if (v) cnt += 1;
    }
    std::cout << keys_gen << "/" << cnt << "/" << NTESTS << std::endl;

    crypto_finalize();
    return 0;
}
