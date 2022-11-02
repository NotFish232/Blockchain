#include "src/utils.hpp"
#include "src/network.hpp"
#include <iostream>
#include <openssl/sha.h>

using namespace std;

int main(int argc, char **argv) {
    RSA *keypair = Utils::generate_rsa_keys();
    string msg = "test message";
    string enc = Utils::base64_encode(msg);
    cout << enc << endl;
    string dec = Utils::base64_decode(enc);
    cout << dec << endl;
    Utils::free(keypair);
    return 0;
}