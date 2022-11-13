#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <algorithm>
#include <iomanip>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>

using std::string;

typedef unsigned char uchar;

class Crypto {
private:
    /**
     * @brief helper method to sign message with private key
     * @param msg mesage to sign
     * @param private_key private rsa key used to sign the message
     * @return a string containing the signature of the message
     */
    static string _sign_message(const string &msg, RSA *private_key);

    /**
     * @brief helper method to verify the signature of a method
     * @param msg message to verify autheniticy of
     * @param signature provided signature from signing
     * @param public_key public rsa key used to validate signature
     * @return a boolean of whether or not the signature is legitimate
     */
    static bool _verify_signature(const string &msg, const string &signature, RSA *public_key);

public:
    /**
     * @brief base64 encodes a string
     * @param input string to base64 encode
     * @return a base64 encoded string
    */
    static string base64_encode(const string &input);

    /**
     * @brief base64 decodes a string
     * @param input base64 encoded string to decode
     * @return a base64 decode string with no whitespace
    */
    static string base64_decode(const string &input);

    /**
     * @brief hashes a string with the sha256 algorithim
     * @param input string to hash
     * @return hash representation of string
    */
    static string sha256(const string &input);

    /**
     * @brief frees pointer to an rsa key
     * @param keypair rsa keypair to free
    */
    static void free(RSA *keypair);

    /**
     * @brief generates a RSA keypair
     * @return pointer to a RSA object holding the public and private keys
    */
    static RSA *generate_rsa_keys();


    static char *rsa_encrypt(const char *msg, RSA *public_key);
    static char *rsa_decrypt(const char *msg, RSA *private_key);
    static int export_public_key(const RSA *public_key, const string &file_name = "public", const string &file_path = "./keys/");
    static int export_private_key(const RSA *private_key, const string &file_name = "private", const string &file_path = "./keys/");
    static RSA *import_public_key(const string &file_name = "public", const string &file_path = "./keys/");
    static RSA *import_private_key(const string &file_name = "private", const string &file_path = "./keys/");
    static string sign_message(const string &msg, RSA *private_key);
    static bool verify_signature(const string &msg, const string &signature, RSA *public_key);
};

#endif