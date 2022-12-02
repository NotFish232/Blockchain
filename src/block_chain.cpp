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

Block *BlockChain::get_root() {
    return blocks.size() > 0 ? get_block(0): nullptr;
}

Block *BlockChain::get_block(int index) {
    return index < blocks.size() ? &blocks[index] : nullptr;
}

Block *BlockChain::get_block(const function<bool(const Block &block)> &callable) {
    auto it = find_if(blocks.begin(), blocks.end(), callable);
    return it != blocks.end() ? &(*it) : nullptr;
}

vector<Block> &BlockChain::get_blocks() {
    return blocks;
}

bool BlockChain::delete_block(const function<bool(const Block &block)> &callable) {
    auto it = find_if(blocks.begin(), blocks.end(), callable);
    if (it == blocks.end())
        return false;
    blocks.erase(it);
    return true;
}

ostream &operator<<(ostream &os, const BlockChain &block_chain) {
    for (const auto &block : block_chain.blocks) {
        os << block << '\n';
    }
    return os;
}