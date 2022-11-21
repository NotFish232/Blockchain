#include "../include/block.hpp"

using namespace std;

Block::Block(const std::string &_username, RSA *public_key, RSA *private_key) {
    username = _username;
    _public_key = public_key;
   _private_key = private_key;
}
Block::~Block() {
    Crypto::free(_public_key);
    if (_private_key != nullptr)
        Crypto::free(_private_key);
}
string Block::get_hash() {
    return "placeholder text";
}

string Block::get_username() {
    return username;
}

RSA *Block::get_public() {
    return _public_key;
}

RSA *Block::get_private() {
    return _private_key;
}

ostream &operator<<(ostream &os, const Block &block) {
    //TODO: actually good description of the block;
    os << "User: " << block.username << '\n';
    os << "Url: " << block.url << '\n';
    return os;
}