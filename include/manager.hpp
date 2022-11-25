#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "block_chain.hpp"
#include "client.hpp"
#include "server.hpp"
#include "utils.hpp"
#include <thread>

#define DEFAULT_HOST "ws://localhost"

/**
 * Manages the network and the block chain
 * provides the bulk of the logic
 */
class Manager {
private:
/**
 * @var no_overwrite whether or not to overwrite rsa keys, defaults to true
*/
    bool no_overwrite;
    Server _server;
    std::thread server_thread;
    Client _client;
    std::thread client_thread;
    BlockChain block_chain;

    void save_public_key(const std::string &username, RSA *public_key);
    void send_initial_message();
    void send_sync_message();
    void on_message(const Json::Value &message);
    void on_connect(const std::string &url);
    void on_disconnect(const std::string &url);
    void load_config(const std::string &path = "./config/");
    void save_config(const std::string &path = "./config/");

public:
    Manager(const string &username, const string &url, bool no_overwrite = true);
    ~Manager();
    void run();
    void send_message(const std::string &msg);
    void open_connection(const std::string &url);
};

#endif