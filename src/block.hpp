#ifndef BLOCK_H
#define BLOCK_H

#include "utils.hpp"
#include <iostream>

class Block {
private:
    Block *last_block;
    RSA *key_pair;
    std::string hash;

public:
    Block();
    ~Block();
    std::string get_hash();
};

#endif