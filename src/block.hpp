#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "crypto.hpp"
#include <iostream>

class Block {
private:
    RSA *public_key;
    std::string hash;

public:
    Block();
    ~Block();
    std::string get_hash();
};

#endif