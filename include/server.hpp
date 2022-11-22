#ifndef SERVER_HPP
#define SERVER_HPP

#define ASIO_STANDALONE
#include "utils.hpp"
#include <functional>
#include <iostream>
#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef std::function<void(const Json::Value &)> msg_func;
typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_set;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Server {
private:
    server _server;
    con_set connections;
    msg_func message_callback;
    int port;

    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

public:
    Server(int port);
    ~Server();
    void set_message_callback(const msg_func &callback);
    void run();
};

#endif