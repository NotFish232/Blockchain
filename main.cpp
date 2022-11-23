#include "include/manager.hpp"
#include <cstdlib>
#include <iostream>
#define USER "user0"
#define PORT 8001
// need at least one block to connect to, will sync the rest of the chain after connection
#define URL "ws://localhost:8001"

using namespace std;


int main(int argc, char **argv) {
    string user = USER;
    int port = PORT;
    if (argc >= 3) {
        user = argv[1];
        port = stoi(argv[2]);
    }
    Manager manager(user, port);
    manager.run();
    manager.open_connection(URL);
    while (true) {
    }
    return 0;
}