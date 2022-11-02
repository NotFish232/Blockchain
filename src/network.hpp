#ifndef NETWORK_HPP
#define NETWORK_HPP
#define ASIO_STANDALONE

#include <jsoncpp/json/json.h>
#include <set>
#include <thread>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::client<websocketpp::config::asio> client;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_set;
typedef websocketpp::config::asio::message_type::ptr msg_ptr;

class Network {
private:
    server _server;
    std::thread server_thread;
    std::set<client> _clients;
    con_set connections;
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, msg_ptr msg);
    void run_server();

public:
    // port to listen on
    Network(int port);
    ~Network();
    void run();
    void send_message(std::string msg);
    void add_connection(std::string url);
};

#endif