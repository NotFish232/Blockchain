#include "../include/client.hpp"

using namespace std;

Client::Client(const std::string &_url) {
    url = _url;

    _client.clear_access_channels(websocketpp::log::alevel::frame_header);
    _client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // c.set_error_channels(websocketpp::log::elevel::none);
    _client.init_asio();

    _client.set_open_handler(bind(&Client::on_open, this, ::_1));
    _client.set_fail_handler(bind(&Client::on_fail, this, ::_1));
    _client.set_close_handler(bind(&Client::on_close, this, ::_1));

    error_code ec;
    client::connection_ptr con = _client.get_connection(url, ec);
    if (ec) {
        cout << "Error connecting to `" << url << "` \n";
        cout << ec.message() << "\n";
    } else {
        _client.connect(con);
    }
}

Client::~Client() {
}

void Client::send_message(const string &msg) {
    _client.send(connection, msg, websocketpp::frame::opcode::text);
}

void Client::on_open(websocketpp::connection_hdl hdl) {
    connection = hdl;
    _client.get_alog().write(websocketpp::log::alevel::app, "Opened Connection");
    send_message("Hello World");
    connection_callback(url);
}

void Client::on_fail(websocketpp::connection_hdl hdl) {
    _client.get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

void Client::on_close(websocketpp::connection_hdl hdl) {
    _client.get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
    disconnection_callback(url);
}

void Client::run() {
    _client.run();
}

void Client::set_connection_callback(const func &callback) {
    connection_callback = callback;
}

void Client::set_disconnection_callback(const func &callback) {
    disconnection_callback = callback;
}
