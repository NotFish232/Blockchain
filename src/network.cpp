#include "../include/network.hpp"
using namespace std;

Network::Network() {
}

Network::~Network() {
    cout << "[ calling network destructor] \n";
}

void Network::run() {
    server_thread = thread([this](){ _server.run(); });
    server_thread.detach();
}

void Network::stop() {
}

void Network::set_port(int port) {
    _server.set_port(port);
}
void Network::send_message(const std::string &msg) {
}

void Network::open_connection(const std::string &url) {
}

void Network::set_message_callback(const func &callback) {
    _server.set_message_callback(callback);
}
