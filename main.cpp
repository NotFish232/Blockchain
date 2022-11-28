#include "include/manager.hpp"

#define USER "user1"
#define URL "ws://localhost:8080/"
#define CONNECTION_URL "ws://localhost:8080/"

using namespace std;

// Robert Jenkins' 96 bit Mix Function
// clang-format off
unsigned long get_seed() {
    unsigned long a = clock(), b = time(nullptr), c = getpid();
    a -= b; a -= c; a ^= (c >> 13);
    b -= c; b -= a; b ^= (a << 8);
    c -= a; c -= b; c ^= (b >> 13);
    a -= b; a -= c; a ^= (c >> 12);
    b -= c; b -= a; b ^= (a << 16);
    c -= a; c -= b; c ^= (b >> 5);
    a -= b; a -= c; a ^= (c >> 3);
    b -= c; b -= a; b ^= (a << 10);
    c -= a; c -= b; c ^= (b >> 15);
    return c;
}
// clang-format on

int main(int argc, char **argv) {

    unsigned long seed = get_seed();

    srand(seed);

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

    string description = random_foods[rand() % random_foods.size()].asString();

    Block block(user, url, description);

    Manager manager(&block);
    manager.run();

    if (url != connection_url)
        manager.open_connection(connection_url);

    while (true) {
        // sleep for a random time betweeen 10 and 20 seconds
        this_thread::sleep_for(chrono::seconds(rand() % 10 + 10));

        string location = random_addresses[rand() % random_addresses.size()]["address"].asString();
        manager.update_location(location);
    }

    return 0;
}