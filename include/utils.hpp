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

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define ERROR(exception) std::cout << RED "ERROR: " RESET << exception.what() << '\n'; exit(1)

class Utils {
private:
public:
    static bool file_exists(const std::string &path);
    static std::string read_file(const std::string &path);
    static Json::Value to_json(const std::string &str);
    static std::string to_string(const Json::Value &json);
};

#endif