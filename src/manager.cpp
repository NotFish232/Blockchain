#include "../include/manager.hpp"

using namespace std;

Manager::Manager(const string &user, int port) : _server(port) {
    _server.set_message_callback(bind(&Manager::on_message, this, _1));
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
}

void Manager::open_connection(const string &url) {
    _client.open_connection(url);
    _client.set_connection_callback(bind(&Manager::on_connect, this, _1));
    _client.set_disconnection_callback(bind(&Manager::on_disconnect, this, _1));
    client_thread = thread([this]() { _client.run(); });
    client_thread.detach();
}

void Manager::on_message(const string &message) {
}

void Manager::on_connect(const string &url) {
    _client.send_message("HELLO MORE WORLD!");
}

void Manager::on_disconnect(const string &url) {
}

Json::Value Manager::initial_message() {
    Json::Value msg;
    /* msg["type"] = "init";
     msg["user"] = USER;
     msg["public_key"] = Utils::read_file("./keys/" USER "_public.pem");*/
    return msg;
}