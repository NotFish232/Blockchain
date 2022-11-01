#ifndef NETWORK_HPP
#define NETWORK_HPP
#define ASIO_STANDALONE

#include <thread>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::client<websocketpp::config::asio> client;
typedef websocketpp::config::asio::message_type::ptr msg_ptr;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Network {
private:
    server _server;
    client _client;
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, msg_ptr msg);
    void config_server(std::string port);
    void config_client(std::string url);

public:
    // port to listen on, url to communicate with
    Network(std::string port, std::string url);
    ~Network();
    void run();
    void send(std::string msg);
};

#endif