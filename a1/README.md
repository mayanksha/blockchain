# Pre-requisites

1. Install libressl [portable](https://github.com/libressl-portable/portable). The makefile assumes that libressl gets installed to /usr/local/lib.
2. Make sure you have boost installed (package libboost-dev on ubuntu).

# Compilation

Run make.

# Baseline Task

Implement the validate function for each transaction in txn_t::validate.

This function must check that:

1. SHA256(public_key) = source_addr
2. SHA256(public_key, source_addr, dest_addr, change_addr) = tx_hash
3. sign(tx_hash, tx_sign, public_key) is valid.

See the cryptotest.cpp file for examples of how we can use these functions.

# Challenge Task

Implement the block_t::validate function. You will do this by removing the call to validate_block_helper and replacing it with your own code.

# Running Tests

The program cryptotest.cpp contains unit tests for RSA signing and verification. Run these tests by invoking cryptotest.

    $ ./cryptotest

The program readchain.cpp reads a blockchain and validates it. Example invocation:

    $ ./readchain tests/t1.dat

There are three test input files: t1.dat, t2.dat and t3.dat provided in tests/ sub-folder. The expected output for these three files is shown in t1.txt, t2.txt and t3.txt.

