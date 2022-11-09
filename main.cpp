#include "src/network.hpp"
#include "src/utils.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    string msg = "this message is going to be encoded";
    string enc = Utils::base64_encode(msg);
    cout << enc << endl;
    string dec = Utils::base64_decode(enc);
    cout << dec << endl;
    //  delete[] enc, dec;
    return 0;
}