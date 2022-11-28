#include "include/manager.hpp"
#include <cstdlib>
#include <iostream>
#define USER "user1"
#define URL "ws://localhost:8080/"
#define CONNECTION_URL "ws://localhost:8080/"
using namespace std;

int main(int argc, char **argv) {
    srand(time(0));

    string user = USER;
    string url = URL;
    string connection_url = CONNECTION_URL;
    if (argc >= 4) {
        user = argv[1];
        url = argv[2];
        connection_url = argv[3];
    }

    DEBUG_PRINT("Starting block chain ( Username - `" + user + "`, Url - `" + url + "`, Connection url - `" + connection_url + "` )");


    RSA *keypair = Crypto::generate_rsa_keys();
    Crypto::export_private_key(keypair, user + "_private");
    Crypto::export_public_key(keypair, user + "_public");
    Crypto::free(keypair);

    Json::Value random_foods = Utils::load_json("random-foods");
    Json::Value random_addresses = Utils::load_json("random-addresses");

    string description = random_foods[rand() % random_foods.size()].asString();

    Block block(user, url, description);

    Manager manager(&block);
    manager.run();

    if (url != connection_url)
        manager.open_connection(connection_url);

    while (true) {
        // sleep for a random time betweeen 5 and 10 seconds
        this_thread::sleep_for(chrono::seconds(rand() % 10 + 5));

        string location = random_addresses[rand() % random_addresses.size()]["address"].asString();
        manager.update_location(location);
    }

    return 0;
}