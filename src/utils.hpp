#ifndef UTILS_HPP
#define UTILS_HPP

#include <iomanip>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>

typedef unsigned char uchar;

class Utils {
public:
    // static size_t calculate_base64_length(const std::string &b64_text);
    static std::string base64_encode(const std::string &input);
    static std::string base64_decode(const std::string &input);
    static uchar *_sign_message(const std::string &msg, size_t *enc_msg_len_ptr, RSA *private_key);
    static bool _verify_signature(const std::string &msg, const uchar *hash, size_t hash_len, RSA *public_key);
    static bool file_exists(std::string file_path, std::string file_name);

public:
    static std::string sha256(const std::string &str);
    static void free(RSA *keypair);
    static RSA *generate_rsa_keys();
    static char *rsa_encrypt(const char *msg, RSA *public_key);
    static char *rsa_decrypt(const char *msg, RSA *private_key);
    static int export_public_key(const RSA *public_key, const std::string file_path = "./keys/", const std::string file_name = "public");
    static int export_private_key(const RSA *private_key, const std::string file_path = "./keys/", const std::string file_name = "private");
    static RSA *import_public_key(const std::string file_path = "./keys/", const std::string file_name = "public");
    static RSA *import_private_key(const std::string file_path = "./keys/", const std::string file_name = "private");
    static std::string sign_message(const std::string &msg, RSA *private_key);
    static bool verify_signature(const std::string &msg, const std::string &signature, RSA *pulic_key);
};

#endif