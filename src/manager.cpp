#include "../include/manager.hpp"

using namespace std;

Manager::Manager() {
    network.set_port(PORT);
    cout << initial_message() << endl;
}

Manager::~Manager() {

}

Json::Value Manager::initial_message() {
    Json::Value msg;
    msg["type"] = "init";
    msg["user"] = USER;
    msg["public_key"] = Utils::read_file("./keys/" USER "_public.pem");
    return msg;
}