#include "utils.hpp"

using namespace std;

Json::Value Utils::to_json(const string &str) {
    Json::Reader reader;
	Json::Value result;
	return reader.parse(str, result) ? result: Json::Value();
}

string Utils::to_string(const Json::Value &json) {
    Json::FastWriter fw;
    return fw.write(json);
}