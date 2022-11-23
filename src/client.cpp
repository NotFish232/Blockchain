#include "../include/client.hpp"

using namespace std;

Client::Client() {
    _client.clear_access_channels(websocketpp::log::alevel::all);
    _client.clear_error_channels(websocketpp::log::alevel::all);

    _client.set_open_handler(bind(&Client::on_open, this, _1));
    _client.set_fail_handler(bind(&Client::on_fail, this, _1));
    _client.set_close_handler(bind(&Client::on_close, this, _1));

    _client.init_asio();
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
        DEBUG_PRINT("Error code: `" + ec.message() + "`");
    }
}

void Client::send_all_message(const string &msg) {
    for (auto hdl : connections) {
        string url = get_url(hdl);
        DEBUG_PRINT("Sending message `" + msg + "` TO `" + url + "`");
        _client.send(hdl, msg, websocketpp::frame::opcode::text);
    }
}

void Client::send_message(const string &msg, const string &url) {
    auto pos = find_if(connections.begin(), connections.end(), [&](const auto &hdl) {
        return url == get_url(hdl);
    });
    DEBUG_PRINT("Sending message `" + msg + "` TO `" + url + "`");
    _client.send(*pos, msg, websocketpp::frame::opcode::text);

}

string Client::get_url(const websocketpp::connection_hdl &hdl) {
    auto con = _client.get_con_from_hdl(hdl);
    return con->get_uri()->str();
}

void Client::on_open(websocketpp::connection_hdl hdl) {
    connections.insert(hdl);
    string url = get_url(hdl);
    DEBUG_PRINT("Opened connection with url `" + url + "`");
    connection_callback(url);
}

void Client::on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    string url = get_url(hdl);
    DEBUG_PRINT("Closed connection with url `" + url + "`");
    disconnection_callback(url);
}

void Client::on_fail(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    string url = get_url(hdl);
    DEBUG_PRINT("Failed connection with url `" + url + "`");
}

void Client::run() {
    _client.run();
}

void Client::set_connection_callback(const con_func &callback) {
    connection_callback = callback;
}

void Client::set_disconnection_callback(const con_func &callback) {
    disconnection_callback = callback;
}
