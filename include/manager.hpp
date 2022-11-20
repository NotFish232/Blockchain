#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "block_chain.hpp"
#include "client.hpp"
#include "server.hpp"
#include "utils.hpp"
#include <thread>

/**
 * Manages the network and the block chain
 * provides the bulk of the logic
 */
class Manager {
private:
    Server _server;
    std::thread server_thread;
    Client _client;
    std::thread client_thread;
    BlockChain block_chain;

    // methods to generate messages to send
    Json::Value initial_message();
    void on_message(const std::string &message);
    void on_connect(const std::string &url);
    void on_disconnect(const std::string &url);

public:
    Manager(const std::string &user, int port);
    ~Manager();
    void run();
    void stop();
    void send_message(const std::string &msg);
    void open_connection(const std::string &url);
};

#endif