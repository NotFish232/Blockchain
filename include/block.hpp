#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "crypto.hpp"
#include <iostream>

class Block {
private:
    RSA *public_key, *private_key;
    // if its not first block this field is empty
    std::string username;
    std::string url;
    std::string location;

public:
    Block(const std::string &username, const std::string &url);
    ~Block();
    std::string get_hash() const;
    std::string get_username() const;
    std::string get_url() const;
    std::string get_str_public() const;
    RSA *get_public();
    RSA *get_private();
    friend std::ostream &operator<<(std::ostream &os, const Block &block);
};

#endif