#include "src/utils.hpp"
#include <iostream>
#include <openssl/sha.h>

int main(int argc, char **argv) {
    using namespace std;
    RSA *keypair = Utils::generate_rsa_keys();
    Utils::export_private_key(keypair);
    Utils::export_public_key(keypair);
    char *encrypted = Utils::rsa_encrypt("hello world", keypair);
    RSA *public_key = Utils::import_private_key();
    char *decrypted = Utils::rsa_decrypt(encrypted, public_key);
    cout << decrypted << endl;
    return 0;
}