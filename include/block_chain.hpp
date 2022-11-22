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
    void add_block(const std::string &username, const std::string &url);
    Block *get_block(int index);
    friend std::ostream &operator<<(std::ostream &os, const BlockChain &block_chain);
};

#endif