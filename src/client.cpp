#include "../include/client.hpp"

using namespace std;

Client::Client() {
    _client.clear_access_channels(websocketpp::log::alevel::all);
    _client.clear_error_channels(websocketpp::log::alevel::all);
    //_client.set_access_channels(websocketpp::log::alevel::all);
    //_client.set_error_channels(websocketpp::log::alevel::all);

    _client.set_open_handler(bind(&Client::on_open, this, _1));
    _client.set_fail_handler(bind(&Client::on_fail, this, _1));
    _client.set_close_handler(bind(&Client::on_close, this, _1));

    _client.init_asio();
}

Client::~Client() {
}

void Client::open_connection(const string &url) {
    // don't open a connection to any url that already has one ongoing
    for (const auto &hdl : connections) {
        if (url == get_url_from_hdl(hdl))
            return;
    }
    DEBUG_PRINT("Opening a connection with url " + url);
    error_code ec;
    client::connection_ptr con = _client.get_connection(url, ec);
    if (!ec) {
        _client.connect(con);
    } else {
        DEBUG_PRINT("Error connecting to " + url);
        DEBUG_PRINT("Error: " + ec.message());
    }
}

bool Client::close_connection(const string &url, const string &msg) {
    auto callable = [&](const auto &hdl) {
        return url == get_url_from_hdl(hdl);
    };
    auto it = find_if(connections.begin(), connections.end(), callable);
    if (it == connections.end())
        return false;

    DEBUG_PRINT("Closing connection with " + url);
    _client.pause_reading(*it);
    _client.close(*it, websocketpp::close::status::normal, msg);
    connections.erase(it);
    return true;
}

vector<string> Client::get_connection_urls() {
    vector<string> connection_urls;
    for (const auto &hdl : connections) {
        connection_urls.push_back(get_url_from_hdl(hdl));
    }
    return connection_urls;
}

void Client::send_message_to_all(const string &msg) {

    if (connections.size() == 0)
        return;

    // get string with all urls seperated by commas
    // eliminate excessive debug prints by not printing new message for each url
    string urls = "";
    for (const auto &hdl : connections) {
        urls += get_url_from_hdl(hdl) + ", ";
    }
    urls = urls.substr(0, urls.length() - 2);
    DEBUG_PRINT("Sending message " + msg + " TO " + urls);

    for (const auto &hdl : connections) {
        string url = get_url_from_hdl(hdl);
        error_code ec;
        _client.send(hdl, msg, websocketpp::frame::opcode::text, ec);
        if (ec) {
            DEBUG_PRINT("Failed sending message to " + url);
            DEBUG_PRINT("Error: " + ec.message());
        }
    }
}

void Client::send_message(const string &msg, const string &url) {
    auto pos = find_if(connections.begin(), connections.end(), [&](const auto &hdl) {
        return url == get_url_from_hdl(hdl);
    });
    DEBUG_PRINT("Sending message " + msg + " TO " + url);
    _client.send(*pos, msg, websocketpp::frame::opcode::text);
}

string Client::get_url_from_hdl(const websocketpp::connection_hdl &hdl) {
    auto con = _client.get_con_from_hdl(hdl);
    return con->get_uri()->str();
}

void Client::on_open(websocketpp::connection_hdl hdl) {
    connections.insert(hdl);
    string url = get_url_from_hdl(hdl);
    DEBUG_PRINT("Opened connection with url " + url);
    connection_callback(url);
}

void Client::on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    string url = get_url_from_hdl(hdl);
    DEBUG_PRINT("Closed connection with url " + url);
    disconnection_callback(url);
}

void Client::on_fail(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
    string url = get_url_from_hdl(hdl);
    DEBUG_PRINT("Failed connection with url " + url);
}

void Client::run() {
    _client.run();
}

void Client::set_connection_callback(const function<void(const string &)> &callback) {
    connection_callback = callback;
}

void Client::set_disconnection_callback(const function<void(const string &)> &callback) {
    disconnection_callback = callback;
}
