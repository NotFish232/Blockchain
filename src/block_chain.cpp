#include "../include/block_chain.hpp"

BlockChain::BlockChain() {
}

BlockChain::~BlockChain() {
}

void BlockChain::add_block(const string &username, const string &url) {
    blocks.emplace_back(username, url);
}

Block *BlockChain::get_block(int index) {
    return &blocks[index];
}

ostream &operator<<(ostream &os, const BlockChain &block_chain) {
    for (auto block: block_chain.blocks) {
        os << block << '\n';
    }
    return os;
}