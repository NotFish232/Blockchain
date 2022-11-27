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
    /**
     * @brief block must not already exist if method is called
    */
    void add_block(const string &username, const string &url);
    Block *get_block(int index);
    friend std::ostream &operator<<(std::ostream &os, const BlockChain &block_chain);
};

#endif