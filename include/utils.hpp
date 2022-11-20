#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <sys/stat.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << (x) << "\n"
#else
#define DEBUG_PRINT(x)
#endif

class Utils {
private:
public:
    static bool file_exists(const std::string &path);
    static std::string read_file(const std::string &path);
    static Json::Value to_json(const std::string &str);
    static std::string to_string(const Json::Value &json);
};

#endif