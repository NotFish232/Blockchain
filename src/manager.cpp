#include "../include/manager.hpp"

using namespace std;

Manager::Manager(const string &username, const string &url, bool no_overwrite)
    : _server(Utils::get_port_from_url(url)) {
    // remove port from url

    this->no_overwrite = no_overwrite;
    // block 0 in the block chain must be you
    _server.set_message_callback(bind(&Manager::on_message, this, _1));
    _client.set_connection_callback(bind(&Manager::on_connect, this, _1));
    _client.set_disconnection_callback(bind(&Manager::on_disconnect, this, _1));

    block_chain.add_block(username, url);
}

Manager::~Manager() {
    DEBUG_PRINT("manager destructor called");
}

void Manager::run() {
    server_thread = thread([this]() { _server.run(); });
    server_thread.detach();
}

void Manager::save_public_key(const string &username, RSA *public_key) {
    DEBUG_PRINT("Saving public key for `" + username + "`");
    if (Utils::file_exists("./keys/" + username + "_public.pem")) {
        if (no_overwrite) {
            DEBUG_PRINT("Key `" + username + ".pem` already exists. aborting");
            return;
        }
        cout << "Warning: file `" + username + "_public.pem already exists \n";
        cout << "Would you like to overwrite it? (y / n) ";
        string input;
        cin >> input;
        // if input is yes to overwrite, export anyways
        if (input == "y") {
            DEBUG_PRINT("Saving key `" + username + ".pem`");
            Crypto::export_public_key(public_key, username + "_public");
        }
    } else {
        DEBUG_PRINT("Saving key `" + username + ".pem`");
        Crypto::export_public_key(public_key, username + "_public");
    }
}

void Manager::send_message(const string &message) {
    Json::Value msg;
    msg["type"] = "message";
    msg["username"] = block_chain.get_block(0)->get_username();
    msg["body"] = message;
    msg["signature"] = "";
    _client.send_all_message(Utils::to_string(msg));
}

void Manager::open_connection(const string &url) {
    _client.open_connection(url);
    client_thread = thread([this]() { _client.run(); });
    client_thread.detach();
}

void Manager::on_message(const Json::Value &json) {
    if (!json.isMember("type")) {
        DEBUG_PRINT("Malformed message (doesn't contain type");
        return;
    }

    string type = json["type"].asString();

    if (type == "initialize_block") {
        if (!json.isMember("username") || !json.isMember("public_key") ||
            !json.isMember("hash") || !json.isMember("signature")) {
            DEBUG_PRINT("Malformed message (doesn't contain username, public key, hash, or signature)");
            return;
        }
        string username = json["username"].asString();

        RSA *public_key = Crypto::public_from_string(json["public_key"].asString());

        if (Crypto::verify_signature(json["hash"].asString(), json["signature"].asString(), public_key)) {
            DEBUG_PRINT("Signature of message is valid");
        } else {
            DEBUG_PRINT("Signature of message is not valid");
            // if signature isn't valid don't add the block
            return;
        }

        DEBUG_PRINT("trying to save public key");
        save_public_key(username, public_key);
        DEBUG_PRINT("saved public key");

        Crypto::free(public_key);
        DEBUG_PRINT("freed public key");

        block_chain.add_block(username, json["url"].asString());
        DEBUG_PRINT("added block");

        send_sync_message();
    } else if (type == "sync_blocks") {
        if (!json.isMember("blocks")) {
            DEBUG_PRINT("Malformed message (doesn't contain blocks)");
            return;
        }
        for (const auto &json_block : json["blocks"]) {
            string username = json_block["username"].asString();
            RSA *public_key = Crypto::public_from_string(json_block["public_key"].asString());
            save_public_key(username, public_key);
            Crypto::free(public_key);
            block_chain.add_block(username, json_block["url"].asString());
        }
        DEBUG_PRINT("Finished syncing blocks");
    } else {
        DEBUG_PRINT("Unrecognized type: `" + type + "`");
    }
}

void Manager::on_connect(const string &url) {
    send_initial_message();
}

void Manager::on_disconnect(const string &url) {
}

void Manager::send_initial_message() {
    // initial message to add yourself to the blockchain
    auto *block = block_chain.get_block(0);
    Json::Value msg;
    msg["type"] = "initialize_block";
    msg["username"] = block->get_username();
    msg["hash"] = block->get_hash();
    msg["url"] = block->get_url();
    msg["signature"] = Crypto::sign_message(block->get_hash(), block->get_private());
    msg["public_key"] = block->get_str_public();

    _client.send_all_message(Utils::to_string(msg));
}

void Manager::send_sync_message() {
    // when you receieve a block
    // everybody should send a message with all the blocks in their blockchain
    // make sure everybody is all synced up

    DEBUG_PRINT("trying to send sync message");

    Json::Value msg;
    msg["type"] = "sync_blocks";

    Json::Value blocks;

    for (int i = 0; i < block_chain.get_block_count(); ++i) {
        auto *block = block_chain.get_block(i);
        Json::Value json_block;

        json_block["username"] = block->get_username();
        json_block["url"] = block->get_url();
        json_block["public_key"] = block->get_str_public();

        blocks[i] = json_block;
    }

    msg["blocks"] = blocks;

    _client.send_all_message(Utils::to_string(msg));
}
