#include "include/manager.hpp"
#include <cstdlib>
#include <iostream>
#define USER "user1"
#define URL "ws://localhost:8080/"
#define CONNECTION_URL "ws://localhost:8080/"
using namespace std;

int main(int argc, char **argv) {
    string user = USER;
    string url = URL;
    string connection_url = CONNECTION_URL;
    if (argc >= 4) {
        user = argv[1];
        url = argv[2];
        connection_url = argv[3];
    }

    Manager manager(user, url);
    manager.run();

    manager.open_connection(connection_url);
    //this_thread::sleep_for(chrono::milliseconds(1000));
    while (true) {
        // this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}