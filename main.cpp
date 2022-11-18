#include "include/manager.hpp"
#include <iostream>
#define USER "user_0"
#define PORT 8080

using namespace std;

int main(int argc, char **argv) {
    Manager manager(USER, PORT);
    manager.run();
    manager.open_connection("ws://localhost:8080");
    int x;
    while (true);
    return 0;
}