#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>

/**
 * @brief class to hold data about the position of a node in the blockchain
 */
class Position {
private:
    int latitude, longitude;
    std::string address;

public:
    Position();
    Position(std::string address);
    ~Position();
    void set_position(std::string address);
};

#endif