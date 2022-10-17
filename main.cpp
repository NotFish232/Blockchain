#include "src/utils.hpp"
#include <iostream>
#include <openssl/sha.h>

int main(int argc, char **argv) {
    using namespace std;
    RSA *private_key = Utils::import_private_key();
    RSA *public_key = Utils::import_public_key();
    string msg = "random message";
    char *signature = Utils::sign_message(msg, private_key);
    cout << signature << endl;
    if (Utils::verify_signature(msg, signature, public_key)) {
        cout << "Authentic" << endl;
    } else {
        cout << "not authentic" << endl;
    }
    RSA_free(public_key);
    RSA_free(private_key);
    delete signature;
    return 0;

    /*RSA *keypair = Utils::generate_rsa_keys();
    string msg = "Hello World!";
    char * signed_msg = Utils::rsa_sign(msg, keypair);
    cout << signed_msg << endl;
    RSA_free(keypair);
    delete signed_msg;
    return 0;*/
}