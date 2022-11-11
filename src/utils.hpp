#ifndef UTILS_HPP
#define UTILS_HPP

#include <jsoncpp/json/json.h>

class Utils {
private:
public:
    static Json::Value to_json(const std::string &input);
    static std::string to_string(const Json::Value &input);
};

#endif