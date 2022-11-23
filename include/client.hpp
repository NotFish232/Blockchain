#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utils.hpp"
#include <functional>
#include <iostream>
#include <set>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

typedef std::function<void(const std::string &)> con_func;
typedef websocketpp::client<websocketpp::config::asio> client;
typedef websocketpp::config::asio::message_type::ptr message_ptr;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_set;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class Client {
private:
    client _client;
    con_set connections;
    con_func connection_callback, disconnection_callback;
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);

    /**
     * @brief converts a connection_hdl to an url
     * @param hdl instance of websocketpp::connection_hdl
     * @return string representation of url
    */
    std::string get_url(const websocketpp::connection_hdl &hdl);

public:
    Client();
    ~Client();

    /**
     * @brief tries to open connection to url
     * @param url url to try and connect o
    */
    void open_connection(const std::string &url);
    void send_all_message(const std::string &msg);
    void send_message(const std::string &msg, const std::string &url);
    void set_connection_callback(const con_func &callback);
    void set_disconnection_callback(const con_func &callback);
    void run();
};

#endif