#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "crypto.hpp"
#include <iostream>

class Block {
private:
    RSA *_public_key, *_private_key;
    // if its not first block this field is empty
    std::string hash;
    std::string username;
    std::string url;
    std::string location;

public:
    Block(const std::string &_username, RSA *public_key, RSA *private_key = nullptr);
    ~Block();
    std::string get_hash();
    std::string get_username();
    RSA *get_public();
    RSA *get_private();
    friend std::ostream &operator<<(std::ostream &, const Block &block);
};

#endif