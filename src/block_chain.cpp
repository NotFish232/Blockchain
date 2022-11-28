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

void BlockChain::add_block(const string &username, const string &url, const string &description) {

    blocks.emplace_back(username, url, description);
    DEBUG_PRINT("Added block to block chain, with username `" + username + "`");
}

Block *BlockChain::get_block(int index) {
    return index < blocks.size() ? &blocks[index] : nullptr;
}

Block *BlockChain::get_block_by_username(const string &username) {
    for (auto &block: blocks) {
        if (block.get_username() == username) {
            return &block;
        }
    }
    return nullptr;
}

ostream &operator<<(ostream &os, const BlockChain &block_chain) {
    for (auto block : block_chain.blocks) {
        os << block << '\n';
    }
    return os;
}