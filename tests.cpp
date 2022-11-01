#include "src/crypto.hpp"
#include <iostream>

#define assert(condition)                                                                       \
    do {                                                                                        \
        if (condition) {                                                                        \
            cout << "Assertion `" #condition "` succeeded! \n";                                 \
        } else {                                                                                \
            cout << "[ERROR] Assertion `" #condition "` failed on line " << __LINE__ << ". \n"; \
        }                                                                                       \
    } while (false)

using namespace std;

int main(int argc, char **argv) {
    cout << "Running test suite...\n";
    RSA *private_key = import_private_key("./keys/", "user0_private");
    RSA *public_key = import_public_key("./keys/", "user0_public");
    assert(private_key != NULL);
    assert(public_key != NULL);
    char *msg = "this message is going to be encoded";
    char *encrypted_msg = rsa_encrypt(msg, public_key);
    char *decrypted_msg = rsa_decrypt(encrypted_msg, private_key);
    assert(string{msg} == string{decrypted_msg});
    delete[] encrypted_msg, decrypted_msg;
    char *signed_msg = sign_message(msg, private_key);
    assert(verify_signature(msg, signed_msg, public_key));
    return 0;
}