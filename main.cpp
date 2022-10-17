#include "src/utils.hpp"
#include <iostream>
#include <openssl/sha.h>

int main(int argc, char **argv) {
    using namespace std;
    RSA *keypair = Utils::generate_rsa_keys();
    Utils::export_private_key(keypair);
    Utils::export_public_key(keypair);
    RSA *private_key = Utils::import_private_key();
    RSA *public_key = Utils::import_public_key();
    string msg = "random message";
    char *encrypted_message = Utils::rsa_encrypt(msg.c_str(), public_key);
    cout << encrypted_message << endl;
    char *decrypted_message = Utils::rsa_decrypt(encrypted_message, private_key);
    cout << decrypted_message << endl;
    RSA_free(public_key);
    RSA_free(private_key);
    return 0;

    /*RSA *keypair = Utils::generate_rsa_keys();
    string msg = "Hello World!";
    char * signed_msg = Utils::rsa_sign(msg, keypair);
    cout << signed_msg << endl;
    RSA_free(keypair);
    delete signed_msg;
    return 0;*/
}