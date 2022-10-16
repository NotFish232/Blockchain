#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include "block.hpp"
#include <iostream>
#include <vector>

using namespace std;

class BlockChain {
private:
    vector<Block *> block_chain;

public:
    BlockChain();
    ~BlockChain();
    Block *get_block(size_t index);
};

#endif