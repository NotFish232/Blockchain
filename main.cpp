#include "include/manager.hpp"
#include <iostream>
#define USER "user0"
#define PORT 8080
// need at least one block to connect to, will sync the rest of the chain after connection
#define URL "ws://localhost:8080"

using namespace std;

int main(int argc, char **argv) {
    Manager manager(USER, PORT);
    manager.run();
    manager.open_connection("ws://localhost:8080");

    while (true) { }
    return 0;
}