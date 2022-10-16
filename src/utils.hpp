#ifndef UTILS_H
#define UTILS_H
#include <iomanip>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#define uchar unsigned char

class Utils {
public:
    static std::string sha256(const std::string str);
    static RSA *gen_rsa_keys();
    static char *rsa_encrypt(const char *msg, RSA *keypair);
    static char *rsa_decrypt(const char *msg, RSA *keypair);
};
#endif