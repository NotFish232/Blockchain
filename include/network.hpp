#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "client.hpp"
#include "server.hpp"
#include <jsoncpp/json/json.h>
#include <set>
#include <thread>

class Network {
private:
    std::mutex mtx;
    struct io_lock {
        std::lock_guard<std::mutex> l;
        io_lock(std::mutex &mtx) : l(mtx) {}
        friend std::ostream &operator<<(std::ostream &os, io_lock const &lock) {
            return os;
        }
    };
    /**
     * server and server thread to run the server and listen
     * set of connections that are connected to the server
     * set of clients to connect to other servers
     * each client has a thread that runs its loop
     */
    Server _server;
    std::thread server_thread;
    std::set<std::pair<Client, std::thread>> _clients;

public:
    // port to listen on
    Network(int port);
    ~Network();
    void run();
    void stop();
    void send_message(const std::string &msg);
    void add_connection(const std::string &url);
};

#endif