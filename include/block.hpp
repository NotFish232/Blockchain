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
    void set_username(const string &username);
    string get_url() const;
    void set_url(const string &url);
    string get_description() const;
    void set_description(const string &description);
    string get_location() const;
    void set_location(const string &location);
    string get_str_public() const;
    RSA *get_public();
    RSA *get_private();
    friend std::ostream &operator<<(std::ostream &os, const Block &block);
};

#endif