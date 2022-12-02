#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "block.hpp"
#include <functional>
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
    Block *get_root();
    Block *get_block(int index);
    Block *get_block(const std::function<bool(const Block &block)> &callable);
    bool delete_block(const std::function<bool(const Block &block)> &callable);
    vector<Block> &get_blocks();
    friend std::ostream &operator<<(std::ostream &os, const BlockChain &block_chain);
};

#endif