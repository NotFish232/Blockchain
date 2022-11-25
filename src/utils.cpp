#include "../include/utils.hpp"

using namespace std;

int Utils::get_port_from_url(const string &url) {
    int port_start = url.find_last_of(":") + 1;
    int port_end = url.find("/", port_start);
    if (port_end == string::npos)
        port_end = url.length();
    return stoi(url.substr(port_start, port_end - port_start));
}

bool Utils::file_exists(const string &path) {
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
}

string Utils::read_file(const string &path) {
    if (!file_exists(path))
        return NULL;
    fstream f(path);
    stringstream ss;
    ss << f.rdbuf();
    f.close();
    return ss.str();
}

Json::Value Utils::to_json(const string &str) {
    Json::Reader reader;
    Json::Value result;
    return reader.parse(str, result) ? result : Json::Value();
}

string Utils::to_string(const Json::Value &json) {
    Json::StyledWriter sw;
    return sw.write(json);
}