#include "../include/block.hpp"

using namespace std;

Block::Block(const std::string &username, const std::string &url, const string &description) {
    this->username = username;
    this->url = url;
    this->description = description;
    this->location = "None";

    public_key = Crypto::import_public_key(username + "_public");
    private_key = Crypto::import_private_key(username + "_private");
}
Block::~Block() {
    Crypto::free(public_key);
    if (private_key != nullptr)
        Crypto::free(private_key);
}
string Block::get_hash() const {
    return Crypto::sha256(username + url + description + location);
}

string Block::get_username() const {
    return username;
}

void Block::set_username(const string &username) {
    this->username = username;
}

string Block::get_url() const {
    return url;
}

void Block::set_url(const string &url) {
    this->url = url;
}

string Block::get_description() const {
    return description;
}

void Block::set_description(const string &description) {
    this->description = description;
}

string Block::get_location() const {
    return location;
}

void Block::set_location(const string &location) {
    this->location = location;
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
    os << "Block(username = \"" << block.username;
    os << "\", url = \"" << block.url;
    os << "\", location = \"" << block.location;
    os << "\", description = \"" << block.description << "\")";
    return os;
}