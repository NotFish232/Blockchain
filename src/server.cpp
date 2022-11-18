#include "../include/server.hpp"

using namespace std;

Server::Server(int port) {
    _server.set_error_channels(websocketpp::log::elevel::all);
    _server.set_access_channels(websocketpp::log::alevel::frame_payload);
    _server.set_open_handler(bind(&Server::on_open, this, ::_1));
    _server.set_fail_handler(bind(&Server::on_fail, this, ::_1));
    _server.set_close_handler(bind(&Server::on_close, this, ::_1));
    _server.set_message_handler(bind(&Server::on_message, this, _1, _2));
    _server.init_asio();
    _server.listen(port);
    _server.start_accept();
}

Server::~Server() {
    for (websocketpp::connection_hdl con : connections) {
        _server.close(con, websocketpp::close::status::blank, "");
    }
    _server.stop_listening();
    _server.stop();
}

void Server::on_open(websocketpp::connection_hdl hdl) {
    connections.insert(hdl);
}

void Server::on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
}

void Server::on_fail(websocketpp::connection_hdl hdl) {
}

void Server::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    message_callback(msg->get_payload());
}

void Server::run() {
    _server.run();
}

void Server::set_message_callback(const func &callback) {
    message_callback = callback;
}
