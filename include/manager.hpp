#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "block_chain.hpp"
#include "client.hpp"
#include "server.hpp"
#include "utils.hpp"
#include <websocketpp/common/thread.hpp>

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
    thread server_thread;
    Client _client;
    thread client_thread;
    BlockChain block_chain;

    void save_public_key(const string &username, RSA *public_key);
    void send_initial_message(const string &url);
    void send_sync_message();
    void on_message(const Json::Value &message);
    void on_connect(const string &url);
    void on_disconnect(const string &url);
    void load_config(const string &path = "./config/");
    void save_config(const string &path = "./config/");

public:
    Manager(const string &username, const string &url, bool no_overwrite = true);
    ~Manager();
    void run();
    void send_message(const string &msg);
    void open_connection(const string &url);
};

#endif