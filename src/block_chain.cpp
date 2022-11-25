#include "../include/block_chain.hpp"

BlockChain::BlockChain() {
    blocks.reserve(20);
}

BlockChain::~BlockChain() {
}

bool BlockChain::block_exists(const std::string &username) const {
    auto callable = [&username](const Block &block) {
        return block.get_username() == username;
    };
    return find_if(blocks.begin(), blocks.end(), callable) != blocks.end();
}

int BlockChain::get_block_count() const {
    return blocks.size();
}

void BlockChain::add_block(const string &username, const string &url) {

    // if username doesn't already exist in block, add it to chain
    if (!block_exists(username)) {
        blocks.emplace_back(username, url);
        DEBUG_PRINT("Added block to chain, with username `" + username + "`");
    }
}

Block *BlockChain::get_block(int index) {
    return &blocks[index];
}

ostream &operator<<(ostream &os, const BlockChain &block_chain) {
    for (auto block : block_chain.blocks) {
        os << block << '\n';
    }
    return os;
}