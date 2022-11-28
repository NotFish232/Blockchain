#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "crypto.hpp"
#include <iostream>

class Block {
private:
    RSA *public_key, *private_key;
    // if its not first block this field is empty
    string username;
    string url;
    string description;
    string location;

public:
    Block(const string &username, const string &url, const string &decription = "");
    ~Block();
    string get_hash() const;
    string get_username() const;
    string get_url() const;
    string get_description() const;
    string get_location() const;
    void set_location(const string &location);
    string get_str_public() const;
    RSA *get_public();
    RSA *get_private();
    friend std::ostream &operator<<(std::ostream &os, const Block &block);
};

#endif