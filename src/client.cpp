#include "../include/client.hpp"

using namespace std;

Client::Client(const std::string &_url) : url(_url) {
    _client.clear_access_channels(websocketpp::log::alevel::all);
    _client.clear_error_channels(websocketpp::log::alevel::all);
    _client.init_asio();

    _client.set_open_handler(bind(&Client::on_open, this, ::_1));
    _client.set_fail_handler(bind(&Client::on_fail, this, ::_1));
    _client.set_close_handler(bind(&Client::on_close, this, ::_1));

    error_code ec;
    client::connection_ptr con = _client.get_connection(url, ec);
    if (!ec) {
        _client.connect(con);
    } else {
        DEBUG_PRINT("Error connecting to `" + url + "`");
        DEBUG_PRINT("MESSAGE: `" + ec.message() + "`");
    }
}

Client::~Client() {
}

void Client::send_message(const string &msg) {
    if (!connection.expired()) {
        DEBUG_PRINT("SENDING MESSAGE `" + msg + "` TO `" + url + "`");
        _client.send(connection, msg, websocketpp::frame::opcode::text);
    } else {
        DEBUG_PRINT("TRIED TO SEND MESSAGE `" + msg + "` BUT CONNECTION INVALID");
    }
}

void Client::on_open(websocketpp::connection_hdl hdl) {
    connection = hdl;
    DEBUG_PRINT("OPENED CONNECTION WITH URL `" + url + "`");
    connection_callback(url);
}

void Client::on_fail(websocketpp::connection_hdl hdl) {
    DEBUG_PRINT("FAILED CONNECTION WITH URL `" + url + "`");
}

void Client::on_close(websocketpp::connection_hdl hdl) {
    DEBUG_PRINT("CLOSED CONNECTION WITH URL `" + url + "`");
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
