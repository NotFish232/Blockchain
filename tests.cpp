#include "include/crypto.hpp"
#include "include/utils.hpp"
#include <chrono>
#include <iostream>

#define private_key_name "user0_private"
#define public_key_name "user0_public"

#define assert(condition)                                                                                      \
    do {                                                                                                       \
        if (condition) {                                                                                       \
            cout << GREEN "[SUCCESS]" RESET " Assertion `" #condition "` succeeded! \n";                       \
            passing_tests++;                                                                                   \
        } else {                                                                                               \
            cout << RED "[FAILURE]" RESET " Assertion `" #condition "` failed on line " << __LINE__ << ". \n"; \
        }                                                                                                      \
        total_tests++;                                                                                         \
    } while (false)

using namespace std;

int main(int argc, char **argv) {
    cout << "Running test suite...\n";
    int passing_tests = 0, total_tests = 0;
    const char *msg = "th1s m3ss@ge !s g01ng t0 b3 us3d for @ll th3 t3sT c@se5. \
    !t is h0pefu!!y l0n9 @nd c0mp1ic@t3d 3n0ugh to t3st aLl of th3 funcT10n5. \
    1f 0n3 oF tHe fUncti0n5 br3ak5 0n anOth3r Me55a9e. \
    tH3n w3lp. - 嘉嘉 (你好姐姐）";
    const string s_msg = string{msg};

    auto start = chrono::high_resolution_clock::now();

    assert(Crypto::sha256(s_msg) == Crypto::sha256(s_msg));

    string b64 = Crypto::base64_encode(s_msg);
    assert(Crypto::base64_decode(b64) == s_msg);

    RSA *keypair = Crypto::generate_rsa_keys();
    assert(keypair != NULL);
    Crypto::free(keypair);

    assert(Utils::file_exists("./keys/" private_key_name ".pem"));
    assert(Utils::file_exists("./keys/" public_key_name ".pem"));
    assert(!Utils::file_exists("this file should not exist"));

    RSA *private_key = Crypto::import_private_key(private_key_name);
    RSA *public_key = Crypto::import_public_key(public_key_name);
    assert(private_key != NULL);
    assert(public_key != NULL);

    char *encrypted_msg = Crypto::rsa_encrypt(msg, public_key);
    char *decrypted_msg = Crypto::rsa_decrypt(encrypted_msg, private_key);
    assert(strcmp(msg, decrypted_msg) == 0);

    delete[] encrypted_msg, decrypted_msg;
    string signed_msg = Crypto::sign_message(msg, private_key);
    assert(Crypto::verify_signature(msg, signed_msg, public_key));

    assert(Crypto::export_private_key(private_key, private_key_name));
    assert(Crypto::export_public_key(public_key, public_key_name));

    Crypto::free(private_key);
    Crypto::free(public_key);


    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Tests passed: [";
    cout << (passing_tests == total_tests ? GREEN : RED);
    cout << passing_tests << "/" << total_tests;
    cout << RESET "] \n";
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
    return 0;
}
