#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "block.hpp"
#include <iostream>
#include <vector>

using namespace std;

class BlockChain {
private:
    vector<Block> blocks;

public:
    BlockChain();
    ~BlockChain();
    bool block_exists(const string &username) const;
    int get_block_count() const;
    void add_block(const string &username, const string &url, const string &description);
    Block *get_block(int index);
    Block *get_block_by_username(const string &username);
    friend std::ostream &operator<<(std::ostream &os, const BlockChain &block_chain);
};

#endif