#include "../include/client.hpp"

using namespace std;

Client::Client(const string &url) {
    _client.clear_access_channels(websocketpp::log::alevel::frame_header);
    _client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // c.set_error_channels(websocketpp::log::elevel::none);
    _client.init_asio();

    _client.set_open_handler(bind(&Client::on_open, this, ::_1));
    _client.set_fail_handler(bind(&Client::on_fail,this, ::_1));
    _client.set_close_handler(bind(&Client::on_close, this, ::_1));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = _client.get_connection(url, ec);
    _client.connect(con);
}

Client::~Client() {
}

void Client::on_open(websocketpp::connection_hdl hdl) {
    _client.get_alog().write(websocketpp::log::alevel::app, "Opened Connection");
}

void Client::on_fail(websocketpp::connection_hdl hdl) {
    _client.get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

void Client::on_close(websocketpp::connection_hdl hdl) {
    _client.get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
}

void Client::run() {
    _client.run();
}
