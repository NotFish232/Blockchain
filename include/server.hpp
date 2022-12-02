#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
#include <functional>
#include <iostream>
#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define ASIO_STANDALONE
#define WEBSOCKETPP_STRICT_MASKING

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_set;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Server {
private:
    server _server;
    con_set connections;
    std::function<void(const Json::Value &)> message_callback;
    std::function<void()> connection_callback;
    std::function<void(const string &)> disconnection_callback;
    int port;
    
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

public:
    Server(int port);
    ~Server();
    void set_message_callback(const std::function<void(const Json::Value &)> &callback);
    void set_connection_callback(const std::function<void()> &callback);
    void set_disconnection_callback(const std::function<void(const string &)> &callback);
    void run();
};

#endif