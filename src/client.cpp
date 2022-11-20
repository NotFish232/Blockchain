#include "../include/client.hpp"

using namespace std;

Client::Client() {
    _client.clear_access_channels(websocketpp::log::alevel::all);
    _client.clear_error_channels(websocketpp::log::alevel::all);
    _client.init_asio();

    _client.set_open_handler(bind(&Client::on_open, this, ::_1));
    _client.set_fail_handler(bind(&Client::on_fail, this, ::_1));
    _client.set_close_handler(bind(&Client::on_close, this, ::_1));
}

Client::~Client() {
}

void Client::open_connection(const string &url) {
    error_code ec;
    client::connection_ptr con = _client.get_connection(url, ec);
    if (!ec) {
        _client.connect(con);
    } else {
        DEBUG_PRINT("Error connecting to `" + url + "`");
        DEBUG_PRINT("MESSAGE: `" + ec.message() + "`");
    }
}

void Client::send_message(const string &msg) {
    for (auto hdl : connections) {
        string url = get_url(hdl);
        DEBUG_PRINT("SENDING MESSAGE `" + msg + "` TO `" + url + "`");
        _client.send(hdl, msg, websocketpp::frame::opcode::text);
    }
}

string Client::get_url(websocketpp::connection_hdl hdl) {
    auto con = _client.get_con_from_hdl(hdl);
    return con->get_host() + ":" + to_string(con->get_port());
}

void Client::on_open(websocketpp::connection_hdl hdl) {
    connections.insert(hdl);
    string url = get_url(hdl);
    DEBUG_PRINT("OPENED CONNECTION WITH URL `" + url + "`");
    connection_callback(url);
}

void Client::on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    string url = get_url(hdl);
    DEBUG_PRINT("CLOSED CONNECTION WITH URL `" + url + "`");
    disconnection_callback(url);
}

void Client::on_fail(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    string url = get_url(hdl);
    DEBUG_PRINT("FAILED CONNECTION WITH URL `" + url + "`");
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
