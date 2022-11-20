#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utils.hpp"
#include <functional>
#include <iostream>
#include <set>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

typedef std::function<void(const std::string &)> func;
typedef websocketpp::client<websocketpp::config::asio> client;
typedef websocketpp::config::asio::message_type::ptr message_ptr;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_set;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Client {
private:
    client _client;
    con_set connections;
    func connection_callback;
    func disconnection_callback;
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    std::string get_url(websocketpp::connection_hdl hdl);

public:
    Client();
    ~Client();
    void open_connection(const std::string &url);
    void send_message(const std::string &msg);
    void set_connection_callback(const func &callback);
    void set_disconnection_callback(const func &callback);
    void run();
};

#endif