#include "../include/block_chain.hpp"

BlockChain::BlockChain() {
}

BlockChain::~BlockChain() {
}

Block *BlockChain::get_block(size_t index) {
    return block_chain[index];
}