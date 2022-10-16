#include "src/utils.hpp"
#include <iostream>
#include <openssl/sha.h>

int main(int argc, char **argv) {
    using namespace std;
    RSA *keypair = Utils::gen_rsa_keys();
    Utils::export_private_key(keypair);
    Utils::export_public_key(keypair);
    return 0;
}