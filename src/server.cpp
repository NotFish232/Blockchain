#include "../include/server.hpp"

using namespace std;

Server::Server() {
    _server.set_error_channels(websocketpp::log::elevel::all);
    _server.set_access_channels(websocketpp::log::alevel::frame_payload);

    _server.set_open_handler(bind(&Server::on_open, this, ::_1));
    _server.set_fail_handler(bind(&Server::on_fail, this, ::_1));
    _server.set_close_handler(bind(&Server::on_close, this, ::_1));
    _server.set_message_handler(bind(&Server::on_message, this, _1, _2));
}

Server::~Server() {
}

void Server::on_open(websocketpp::connection_hdl hdl) {
}

void Server::on_close(websocketpp::connection_hdl hdl) {
}

void Server::on_fail(websocketpp::connection_hdl hdl) {
}

void Server::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
}

void Server::run() {
    _server.run();
}

void Server::set_port(int port) {
    _server.init_asio();
    _server.listen(8080);
    _server.start_accept();
}