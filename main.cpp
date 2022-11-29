#include "include/manager.hpp"
#include <random>

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

    DEBUG_PRINT("Starting block chain ( Username - " + user + ", Url - " + url + ", Connection url - " + connection_url + " )");

    RSA *keypair = Crypto::generate_rsa_keys();
    Crypto::export_private_key(keypair, user + "_private");
    Crypto::export_public_key(keypair, user + "_public");
    Crypto::free(keypair);

    Json::Value random_foods = Utils::load_json("random-foods");
    Json::Value random_addresses = Utils::load_json("random-addresses");

    random_device dev;
    mt19937 rng(dev());   

    uniform_int_distribution<mt19937::result_type> food_dist(0, random_foods.size()); 
    uniform_int_distribution<mt19937::result_type> address_dist(0, random_addresses.size());

    Json::Value food = random_foods[static_cast<int>(food_dist(rng))];

    Block block(user, url, food.asString());

    Manager manager(&block);
    manager.run();

    if (url != connection_url)
        manager.open_connection(connection_url);

    auto callable = [&]() {
        while (true) {
            // sleep for a random time betweeen 10 and 20 seconds
            this_thread::sleep_for(chrono::seconds(rand() % 10 + 10));

            Json::Value location = random_addresses[static_cast<int>(address_dist(rng))];
            manager.update_location(location["address"].asString());
        }
    };

    thread t(callable);
    t.join();

    return 0;
}