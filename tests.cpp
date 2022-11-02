#include "src/utils.hpp"
#include <iostream>

#define private_key_name "user0_private"
#define public_key_name "user0_public"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define assert(condition)                                                                                    \
    do {                                                                                                     \
        if (condition) {                                                                                     \
            cout << GREEN "[SUCCESS]" RESET " Assertion `" #condition "` succeeded! \n";                     \
            passing_tests++;                                                                                 \
        } else {                                                                                             \
            cout << RED "[ERROR]" RESET " Assertion `" #condition "` failed on line " << __LINE__ << ". \n"; \
        }                                                                                                    \
        total_tests++;                                                                                       \
    } while (false)

using namespace std;

int main(int argc, char **argv) {
    cout << "Running test suite...\n";
    int passing_tests = 0, total_tests = 0;
    const char *msg = "this message is going to be encoded";
    const string s_msg = string{msg};

    // assert(Utils::base64_decode(Utils::base64_encode(s_msg)) == s_msg);

    RSA *keypair = Utils::generate_rsa_keys();
    assert(keypair != NULL);
    Utils::free(keypair);

    RSA *private_key = Utils::import_private_key("./keys/", private_key_name);
    RSA *public_key = Utils::import_public_key("./keys/", public_key_name);
    assert(private_key != NULL);
    assert(public_key != NULL);

    char *encrypted_msg = Utils::rsa_encrypt(msg, public_key);
    char *decrypted_msg = Utils::rsa_decrypt(encrypted_msg, private_key);
    assert(strcmp(msg, decrypted_msg) == 0);

    delete[] encrypted_msg, decrypted_msg;
    char * signed_msg = Utils::sign_message(msg, private_key);
    assert(Utils::verify_signature(msg, signed_msg, public_key));

    assert(Utils::export_private_key(private_key, "./keys/", private_key_name));
    assert(Utils::export_public_key(public_key, "./keys/", public_key_name));

    cout << "Tests passed: [";
    cout << (passing_tests == total_tests ? GREEN : RED);
    cout << passing_tests << "/" << total_tests;
    cout << RESET << "]" << endl;
    return 0;
}
