#include "../include/manager.hpp"

using namespace std;

Manager::Manager(const Block *block, bool no_overwrite)
    : _server(Utils::get_port_from_url(block->get_url())) {
    // remove port from url

    this->no_overwrite = no_overwrite;
    // block 0 in the block chain must be you
    _server.set_message_callback(bind(&Manager::on_message, this, _1));
    _client.set_connection_callback(bind(&Manager::on_connect, this, _1));
    _client.set_disconnection_callback(bind(&Manager::on_disconnect, this, _1));

    block_chain.add_block(block->get_username(), block->get_url(), block->get_description());
}

Manager::~Manager() {
}

void Manager::run() {
    server_thread = thread([this]() { _server.run(); });
    server_thread.detach();
}

void Manager::save_public_key(const string &username, RSA *public_key) {
    DEBUG_PRINT("Saving public key for " + username);
    if (Utils::file_exists("./keys/" + username + "_public.pem")) {
        if (no_overwrite) {
            return;
        }
        cout << "Warning: file " + username + "_public.pem already exists \n";
        cout << "Would you like to overwrite it? (y / n) ";
        string input;
        cin >> input;
        // if input is yes to overwrite, export anyways
        if (input == "y") {
            Crypto::export_public_key(public_key, username + "_public");
        }
    } else {
        Crypto::export_public_key(public_key, username + "_public");
    }
}
void Manager::on_message(const Json::Value &json) {
    if (!json.isMember("type")) {
        DEBUG_PRINT("Malformed message (doesn't contain type");
        return;
    }

    string type = json["type"].asString();

    if (type == "message") {
    } else if (type == "initialize_block") {
        if (!json.isMember("username") || !json.isMember("public_key") ||
            !json.isMember("hash") || !json.isMember("signature")) {
            DEBUG_PRINT("Malformed message (doesn't contain username, public key, hash, or signature)");
            return;
        }
        string username = json["username"].asString();
        string url = json["url"].asString();

        if (block_chain.block_exists(username)) {
            DEBUG_PRINT("Block " + username + " already exists");
            return;
        }

        RSA *public_key = Crypto::public_from_string(json["public_key"].asString());

        if (Crypto::verify_signature(json["hash"].asString(), json["signature"].asString(), public_key)) {
            DEBUG_PRINT("Signature of message is valid");
        } else {
            DEBUG_PRINT("Signature of message is not valid");
            // if signature isn't valid don't add the block
            Crypto::free(public_key);
            return;
        }

        save_public_key(username, public_key);

        Crypto::free(public_key);

        open_connection(url);
        block_chain.add_block(username, url, json["description"].asString());

        send_sync_message();
    } else if (type == "sync_blocks") {
        if (!json.isMember("blocks")) {
            DEBUG_PRINT("Malformed message (doesn't contain blocks)");
            return;
        }
        for (const auto &json_block : json["blocks"]) {
            string username = json_block["username"].asString();
            string url = json_block["url"].asString();

            if (block_chain.block_exists(username)) {
                DEBUG_PRINT("Block " + username + " already exists");
                continue;
            }

            RSA *public_key = Crypto::public_from_string(json_block["public_key"].asString());
            save_public_key(username, public_key);
            Crypto::free(public_key);

            open_connection(url);
            block_chain.add_block(username, url, json_block["description"].asString());
        }
        DEBUG_PRINT("Finished syncing blocks");
    } else if (type == "update_location") {
        if (!json.isMember("username") || !json.isMember("location") ||
            !json.isMember("hash") || !json.isMember("signature")) {
            DEBUG_PRINT("Malformed message (doesn't contain username, location, hash, or signature)");
            return;
        }

        string username = json["username"].asString();

        auto *block = block_chain.get_block_by_username(username);

        if (block == nullptr) {
            return;
        }

        if (Crypto::verify_signature(json["hash"].asString(), json["signature"].asString(), block->get_public())) {
            DEBUG_PRINT("Signature of message is valid");
        } else {
            DEBUG_PRINT("Signature of message is not valid");
            return;
        }

        DEBUG_PRINT("Updating " + username + " location to " + json["location"].asString());
        block->set_location(json["location"].asString());
    } else {
        DEBUG_PRINT("Unrecognized message type: " + type);
    }
}

void Manager::send_message(const string &message) {
    auto *block = block_chain.get_block(0);
    Json::Value msg;
    msg["type"] = "message";
    msg["username"] = block->get_username();
    msg["body"] = message;
    msg["signature"] = Crypto::sign_message(message, block->get_private());
    _client.send_message_to_all(Utils::to_string(msg));
}

void Manager::open_connection(const string &url) {
    _client.open_connection(url);
    client_thread = thread([this]() { _client.run(); });
    client_thread.detach();
}

void Manager::update_location(const string &location) {
    auto *block = block_chain.get_block(0);
    block->set_location(location);

    Json::Value msg;
    msg["type"] = "update_location";
    msg["username"] = block->get_username();
    msg["location"] = block->get_location();
    msg["hash"] = block->get_hash();
    msg["signature"] = Crypto::sign_message(block->get_hash(), block->get_private());
    _client.send_message_to_all(Utils::to_string(msg));
}

void Manager::on_connect(const string &url) {
    send_initial_message(url);
}

void Manager::on_disconnect(const string &url) {
}

void Manager::send_initial_message(const string &url) {
    // initial message to add yourself to the blockchain
    auto *block = block_chain.get_block(0);

    Json::Value msg;
    msg["type"] = "initialize_block";
    msg["username"] = block->get_username();
    msg["url"] = block->get_url();
    msg["description"] = block->get_description();
    msg["hash"] = block->get_hash();
    msg["signature"] = Crypto::sign_message(block->get_hash(), block->get_private());
    msg["public_key"] = block->get_str_public();

    _client.send_message(Utils::to_string(msg), url);
}

void Manager::send_sync_message() {
    // when you receieve a block
    // everybody should send a message with all the blocks in their blockchain
    // make sure everybody is all synced up

    Json::Value msg;
    msg["type"] = "sync_blocks";

    Json::Value blocks;

    for (int i = 0; i < block_chain.get_block_count(); ++i) {
        auto *block = block_chain.get_block(i);
        Json::Value json_block;

        json_block["username"] = block->get_username();
        json_block["url"] = block->get_url();
        json_block["description"] = block->get_description();
        json_block["public_key"] = block->get_str_public();

        blocks[i] = json_block;
    }

    msg["blocks"] = blocks;

    _client.send_message_to_all(Utils::to_string(msg));
}

ostream &operator<<(ostream &os, Manager &manager) {
    for (int i = 0; i < manager.block_chain.get_block_count(); ++i) {
        auto *block = manager.block_chain.get_block(i);
        cout << *block << '\n';
    }
    return os;
}
