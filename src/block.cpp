#include "../include/block.hpp"

using namespace std;

Block::Block(const std::string &username, const std::string &url, const string &description) {
    this->username = username;
    this->url = url;
    this->description = description;
    public_key = Crypto::import_public_key(username + "_public");
    private_key = Crypto::import_private_key(username + "_private");
}
Block::~Block() {
    Crypto::free(public_key);
    if (private_key != nullptr)
        Crypto::free(private_key);
}
string Block::get_hash() const {
    return Crypto::sha256(username + url);
}

string Block::get_username() const {
    return username;
}

string Block::get_url() const {
    return url;
}

string Block::get_description() const {
    return description;
}

string Block::get_str_public() const {
    return Utils::read_file("./keys/" + username + "_public.pem");
}

RSA *Block::get_public() {
    return public_key;
}

RSA *Block::get_private() {
    return private_key;
}

ostream &operator<<(ostream &os, const Block &block) {
    //TODO: actually good description of the block;
    os << "User: " << block.username << '\n';
    os << "Url: " << block.url << '\n';
    return os;
}