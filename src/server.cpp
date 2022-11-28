#include "../include/server.hpp"

using namespace std;

Server::Server(int port) {
    this->port = port;

    _server.clear_access_channels(websocketpp::log::elevel::all);
    _server.clear_error_channels(websocketpp::log::alevel::all);
    //_server.set_access_channels(websocketpp::log::alevel::all);
    //_server.set_error_channels(websocketpp::log::alevel::all);

    _server.set_open_handler(bind(&Server::on_open, this, _1));
    _server.set_fail_handler(bind(&Server::on_fail, this, _1));
    _server.set_close_handler(bind(&Server::on_close, this, _1));
    _server.set_message_handler(bind(&Server::on_message, this, _1, _2));
}

Server::~Server() {
    for (const auto &hdl : connections) {
        _server.pause_reading(hdl);
        _server.close(hdl, websocketpp::close::status::normal, "");
    }
    _server.stop_listening();
    _server.stop();
}

void Server::on_open(websocketpp::connection_hdl hdl) {
    DEBUG_PRINT("Opened connection on port " + to_string(port));
    connections.insert(hdl);
}

void Server::on_close(websocketpp::connection_hdl hdl) {
    DEBUG_PRINT("Closed connection on port " + to_string(port));
    connections.erase(hdl);
}

void Server::on_fail(websocketpp::connection_hdl hdl) {
    DEBUG_PRINT("Failed connection on port " + to_string(port));
}

void Server::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    Json::Value json = Utils::to_json(msg->get_payload());
    DEBUG_PRINT("Received message: " + Utils::to_string(json));
    message_callback(json);
}

void Server::run() {
    DEBUG_PRINT("Running server on port " + to_string(port));
    _server.set_reuse_addr(true);
    _server.init_asio();
    _server.listen(port);
    _server.start_accept();
    _server.run();
}

void Server::set_message_callback(const msg_func &callback) {
    message_callback = callback;
}
