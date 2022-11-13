#include "../include/network.hpp"

using namespace std;

Network::Network(int port) {
    _server.set_port(port);
}

Network::~Network() {
}

void Network::run() {
    server_thread = thread([this]() { _server.run(); });
}

void Network::stop() {
}

void Network::send_message(const std::string &msg) {
}

void Network::add_connection(const std::string &url) {
}
