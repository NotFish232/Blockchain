#ifndef BLOCK_H
#define BLOCK_H

#include "crypto.hpp"
#include <iostream>
#include <jsoncpp/json/json.h>

class Block {
private:
    Block *last_block;
    RSA *key_pair;
    std::string hash;

public:
    Block();
    ~Block();
    std::string get_hash();
    Json::Value to_json();
};

#endif