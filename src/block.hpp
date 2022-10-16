#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>

using namespace std;

class Block {
private:
    Block *last_block;
    string private_rsa_key;
    string public_rsa_key;
    string hash;

public:
    Block();
    ~Block();
    string get_hash();
};
#endif