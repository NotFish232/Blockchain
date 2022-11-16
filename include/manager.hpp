#ifndef MANAGER_HPP
#define MANAGER_HPP

#define PORT 8080
#define USER "user0"

#include "block_chain.hpp"
#include "network.hpp"
#include "utils.hpp"

/**
 * Manages the network and the block chain
 * provides the bulk of the logic
 */
class Manager {
private:
    Network network;
    BlockChain block_chain;

    // methods to generate messages to send
    Json::Value initial_message();

public:
    Manager();
    ~Manager();
};

#endif