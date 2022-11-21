#include "../include/manager.hpp"

using namespace std;

Manager::Manager(const string &user, int port) : _server(port) {
    // block 0 in the block chain must be you
    _server.set_message_callback(bind(&Manager::on_message, this, _1));
    _client.set_connection_callback(bind(&Manager::on_connect, this, _1));
    _client.set_disconnection_callback(bind(&Manager::on_disconnect, this, _1));

    auto block = Block(user, Crypto::import_public_key(user + "_public"),
                       Crypto::import_private_key(user + "_private"));
    block_chain.add_block(block);
}

Manager::~Manager() {
}

void Manager::run() {
    server_thread = thread([this]() { _server.run(); });
    server_thread.detach();
}

void Manager::stop() {
}

void Manager::send_message(const string &message) {
    Json::Value msg;
    msg["type"] = "message";
    msg["user"] = block_chain.get_block(0)->get_username();
    msg["body"] = message;
    msg["signature"] = "";
    _client.send_message(Utils::to_string(msg));
}

void Manager::open_connection(const string &url) {
    _client.open_connection(url);
    client_thread = thread([this]() { _client.run(); });
    client_thread.detach();
}

void Manager::on_message(const string &message) {
}

void Manager::on_connect(const string &url) {
    send_initial_message();
}

void Manager::on_disconnect(const string &url) {
}

void Manager::send_initial_message() {
    auto *block = block_chain.get_block(0);
    Json::Value msg;
    msg["type"] = "init";
    msg["user"] = block->get_username();
    msg["hash"] = block->get_hash();
   msg["signature"] = Crypto::sign_message(block->get_hash(), block->get_private());

    //FIXME -> rsa key to string
    //msg["public_key"] = Utils::read_file("./keys/" + block->get_username() + "_public.pem");

    _client.send_message(Utils::to_string(msg));

    
}