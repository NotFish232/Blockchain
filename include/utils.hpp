#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/stat.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>

class Utils {
private:
public:
    static bool file_exists(const std::string &path);
    static std::string read_file(const std::string &path);
    static Json::Value to_json(const std::string &str);
    static std::string to_string(const Json::Value &json);
};

#endif