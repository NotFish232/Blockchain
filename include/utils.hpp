#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <sys/stat.h>

#define DEBUG

#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#ifdef DEBUG
#define DEBUG_PRINT(x)                     \
    std::cout << BLUE "[Debug]" RESET " (" \
              << basename(__FILE__) << ", line " << __LINE__ << ") " << (x) << '\n'
#else
#define DEBUG_PRINT(x)
#endif

class Utils {
public:
    static int get_port_from_url(const std::string &url);
    static bool file_exists(const std::string &path);
    static std::string read_file(const std::string &path);
    static Json::Value to_json(const std::string &str);
    static std::string to_string(const Json::Value &json);
};

#endif