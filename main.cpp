#include "include/manager.hpp"
#include <random>

#define USER "user1"
#define URL "ws://localhost:8080/"
#define CONNECTION_URL "ws://localhost:8080/"

using namespace std;

vector<string> split(const string &s, char delim) {
    vector<string> parts;
    stringstream ss(s);
    string part;
    while (getline(ss, part, delim)) {
        parts.push_back(part);
    }

    return parts;
}

void run_interactive(Manager &manager) {
    cout << "\nInteractive shell for managing your node \n";
    cout << "run \"help\" for a list of possible commands \n";
    string input;
    vector<string> arguments;
    while (true) {
        cout << ">> ";
        getline(cin, input);
        arguments = split(input, ' ');

        if (arguments.size() == 0)
            continue;

        string command = arguments[0];

        if (command == "help") {
            if (arguments.size() != 1) {
                cout << "Command \"help\" accepts no arguments \n";
                cout << "For help on a specific command run \"COMMAND help\" \n";
                continue;
            }
            cout << "Commands: \n";
            cout << "   blocks       Manage blocks \n";
            cout << "   connections  Manages connections \n";
            cout << "   keys         Manager RSA keys \n";
            cout << "   location     Update location of block \n";
            cout << "   send         Send message to blocks \n";
            cout << "   help         Shows this help message \n";
            cout << "   exit         Quits program and closes connections \n";
            cout << "For help on a specific command run \"COMMAND help\" \n";
        } else if (command == "blocks") {
            if (arguments.size() == 1) {
                cout << "Command \"blocks\" requires an argument \n";
                cout << "For help on usage run \"blocks help\" \n";
                continue;
            }

            string sub_command = arguments[1];

            if (sub_command == "help") {
                if (arguments.size() != 2) {
                    cout << "Command \"blocks help\" accepts no arguments \n";
                    continue;
                }
                cout << "Commands: \n";
                cout << "   list         Lists all blocks \n";
                cout << "   new          Creates a new block \n";
                cout << "   remove       Removes a block \n";
                cout << "   set          Sets information in a block \n";
                cout << "   help         Shows this help message \n";

            } else if (sub_command == "list") {
                if (arguments.size() != 2) {
                    cout << "Command \"blocks list\" accepts no arguments \n";
                    continue;
                }
                manager.print_blocks();

            } else if (sub_command == "new") {
                if (arguments.size() != 5) {
                    cout << "Command \"blocks new\" requires 3 arguments \n";
                    cout << "Usage:  blocks new USERNAME URL DESCRIPTION \n";
                    continue;
                }
                manager.make_new_block(arguments[2], arguments[3], arguments[4]);
                cout << "Successfully created block \n";

            } else if (sub_command == "remove") {
                if (arguments.size() != 3) {
                    cout << "Command \"blocks remove\" requires an argument \n";
                    cout << "Usage:  blocks remove USERNAME \n";
                    continue;
                }

                string username = arguments[2];

                bool result = manager.delete_block_by_username(username);

                if (result) {
                    cout << "Successfully removed block with username " << username << '\n';
                } else {
                    cout << "Block with username \"" << username << "\" not found \n";
                }

            } else if (sub_command == "set") {
                if (arguments.size() != 5) {
                    cout << "Command \"blocks set\" requires 3 arguments \n";
                    cout << "Usage:  blocks set USERNAME {username, url, location, description} NEW_DATA \n";
                    continue;
                }

                auto *block = manager.get_block_by_username(arguments[2]);

                if (block == nullptr) {
                    cout << "Block with username \"" + arguments[2] + "\" not found \n";
                } else {
                    if (arguments[3] == "username") {
                        block->set_username(arguments[4]);
                        cout << "Successfully updated username \n";
                    } else if (arguments[3] == "url") {
                        block->set_url(arguments[4]);
                        cout << "Successfully updated url \n";
                    } else if (arguments[3] == "location") {
                        block->set_location(arguments[4]);
                        cout << "Successfully updated location \n";
                    } else if (arguments[3] == "description") {
                        block->set_description(arguments[4]);
                        cout << "Successfully updated description \n";
                    } else {
                        cout << "Invalid option \"" + arguments[3] + "\" \n";
                        cout << "Usage:  blocks set {username, url, location, description} NEW_DATA \n";
                    }
                }

            } else {
                cout << "Invalid command \"" + sub_command + "\" \n";
                cout << "For information on commands, run \"blocks help\" \n";
            }

        } else if (command == "connections") {
            if (arguments.size() == 1) {
                cout << "Command \"connections\" requires an argument \n";
                cout << "For help on usage run \"connections help\" \n";
                continue;
            }

        } else if (command == "keys") {

        } else if (command == "location") {

        } else if (command == "send") {

        } else if (command == "exit") {
            if (arguments.size() != 1) {
                cout << "Command \"exit\" accepts no arguments \n";
                continue;
            }
            exit(0);
        } else {
            cout << "Invalid command \"" + command + "\" \n";
            cout << "For information on commands, run \"help\" \n";
        }
    }
}

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

    if (user == "user1") {
        run_interactive(manager);
    } else {
        while (true) {
            // sleep for a random time up to 30 seconds
            this_thread::sleep_for(chrono::milliseconds(rng() % 30'000));

            Json::Value location = random_addresses[static_cast<int>(address_dist(rng))];
            manager.update_location(location["address"].asString());
        }
    }

    return 0;
}