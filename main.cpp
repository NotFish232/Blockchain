#include "src/utils.hpp"
#include "src/network.hpp"
#include <iostream>
#include <openssl/sha.h>

using namespace std;

int main(int argc, char **argv) {
    string msg = "this message is going to be encoded";
    string enc = Utils::base64_encode((uchar*)msg.c_str(), msg.length());
    //uchar* dec = Utils::base64_decode(enc.c_str(), &i);
    //delete[] dec;
    return 0;
}