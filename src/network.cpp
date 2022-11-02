#include "network.hpp"

using namespace std;

void Network::on_open(websocketpp::connection_hdl hdl) {

}

void Network::on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
}

void Network::on_fail(websocketpp::connection_hdl hdl) {
}

void Network::on_message(websocketpp::connection_hdl hdl, msg_ptr msg) {
    // messages come in the form of stringified json
    // type: 'initialize', 'message'
    // user 'name of user'
    // blockchain so syncing data
    // sending keys over websocket? (PUBLIC ONLY)
    // message body will be encrypted
    // with signature as a param
    cout << msg->get_payload() << endl;
}

Network::Network(int port) {
    _server.clear_access_channels(websocketpp::log::alevel::all);
    _server.set_message_handler(bind(&Network::on_message, this, _1, _2));
    _server.init_asio();
    _server.listen(8080);
    _server.start_accept();
}

Network::~Network() {
}

void Network::add_connection(string url) {
}

void Network::run() {
    server_thread = thread([this]() { _server.run(); });
}

void Network::send_message(string msg) {
}
