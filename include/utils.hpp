#ifndef UTILS_HPP
#define UTILS_HPP

#include <jsoncpp/json/json.h>

class Utils {
private:
public:
    static Json::Value to_json(const std::string &str);
    static std::string to_string(const Json::Value &json);
};

#endif