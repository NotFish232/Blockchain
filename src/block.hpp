#ifndef BLOCK_H
#define BLOCK_H
#include "crypto.hpp"
#include <iostream>

using namespace std;

class Block {
private:
    Block *last_block;
    RSA *key_pair;
    string hash;

public:
    Block();
    ~Block();
    string get_hash();
};
#endif