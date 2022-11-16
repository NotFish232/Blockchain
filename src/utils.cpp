#include "../include/utils.hpp"

using namespace std;

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
    Json::FastWriter fw;
    return fw.write(json);
}