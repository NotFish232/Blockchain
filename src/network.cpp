#include "network.hpp"

using namespace std;

void Network::on_open(websocketpp::connection_hdl hdl) {
}

void Network::on_close(websocketpp::connection_hdl hdl) {
}

void Network::on_fail(websocketpp::connection_hdl hdl) {
}

void Network::on_message(websocketpp::connection_hdl hdl, msg_ptr msg) {
}

void Network::config_server(string port) {
}

void Network::config_client(string url) {

}

Network::Network(string port, string url) {
}

Network::~Network() {
}

void Network::run() {
}

void Network::send(string msg) {
}
