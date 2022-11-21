#include "../include/block_chain.hpp"

BlockChain::BlockChain() {
}

BlockChain::~BlockChain() {
}

void BlockChain::add_block(Block &block) {
    blocks.push_back(block);
}

Block *BlockChain::get_block(int index) {
    return &blocks[index];
}

ostream &operator<<(ostream &os, const BlockChain &block_chain) {
    for (auto block: block_chain.blocks) {
        cout << block << '\n';
    }
    return os;
}