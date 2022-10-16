#include "src/utils.hpp"
#include <iostream>
#include <openssl/sha.h>

int main(int argc, char **argv) {
    using namespace std;
    RSA *keypair = Utils::gen_rsa_keys();
    char *text = (char *)"Hello World!, new message";
    char *encrypted_text = Utils::rsa_encrypt(text, keypair);
    string encrypt = string{encrypted_text};
    cout << encrypted_text << endl;
    char *decrypted_text = Utils::rsa_decrypt(encrypted_text, keypair);
    cout << decrypted_text << endl;
    return 0;
}