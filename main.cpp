#include "src/crypto.hpp"
#include <iostream>
#include <openssl/sha.h>

int main(int argc, char **argv) {
    using namespace std;
    RSA *keypair;
    for (int i = 0; i <= 10; ++i) {
        keypair = Crypto::generate_rsa_keys();
        Crypto::export_private_key(keypair, "./keys/", "user" + to_string(i) + "_private");
        Crypto::export_public_key(keypair, "./keys/", "user" + to_string(i) + "_public");
    }
    RSA_free(keypair);
    

    /*RSA *keypair = Utils::generate_rsa_keys();
    string msg = "Hello World!";
    char * signed_msg = Utils::rsa_sign(msg, keypair);
    cout << signed_msg << endl;
    RSA_free(keypair);
    delete signed_msg;
    return 0;*/
}