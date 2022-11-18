#include "../include/manager.hpp"

using namespace std;

Manager::Manager(const string &user, int port) : _server(port) {
    _server.set_message_callback([this](const string &message) { on_message(message); });
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
    Client _client(url);
    _client.set_connection_callback([this](const string &url) { on_connect(url); });
    _client.set_disconnection_callback([this](const string &url) { on_disconnect(url); });
    _client.run();
    _client.send_message("Hello World!");
}

void Manager::on_message(const string &message) {
    cout << "RECEIVED " << message << endl;
}

void Manager::on_connect(const string &url) {
    cout << "connected to `" << url << "` \n";
}

void Manager::on_disconnect(const string &url) {
    cout << "disconnected to `" << url << "` \n";
}

Json::Value Manager::initial_message() {
    Json::Value msg;
    /* msg["type"] = "init";
     msg["user"] = USER;
     msg["public_key"] = Utils::read_file("./keys/" USER "_public.pem");*/
    return msg;
}