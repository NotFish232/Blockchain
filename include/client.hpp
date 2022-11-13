#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

typedef websocketpp::client<websocketpp::config::asio> client;
typedef websocketpp::config::asio::message_type::ptr message_ptr;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Client {
private:
    client _client;
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);

public:
    Client(const std::string &url);
    ~Client();
    void send_message(const std::string &msg);
    void run();
};

#endif