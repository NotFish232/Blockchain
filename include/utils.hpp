#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <functional>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <sys/stat.h>

#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#if defined DEBUG || defined debug
#define DEBUG_PRINT(x)                                                   \
    std::cout << (stringstream()                                         \
                  << BLUE "[Debug]" RESET " ("                           \
                  << basename(__FILE__) << ", line " << __LINE__ << ") " \
                  << (x) << '\n')                                        \
                     .str()
#else
#define DEBUG_PRINT(x)
#endif

using std::string;

class Utils {
public:
    static Json::Value load_json(const string &file_name, const string &file_path = "./config/");
    static int get_port_from_url(const string &url);
    static bool file_exists(const string &path);
    static string read_file(const string &path);
    static Json::Value to_json(const string &str);
    static string to_string(const Json::Value &json);
};

#endif