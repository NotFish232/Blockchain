#ifndef UTILS_H
#define UTILS_H

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>

#define uchar unsigned char

char *base64_encode(const uchar *msg, size_t length);
uchar *base64_decode(const char *b64_msg, size_t *length_ptr);
size_t calculate_base64_length(const char *b64_input);
uchar *rsa_sign(const uchar *msg, size_t msg_len, size_t *enc_msg_len_ptr, RSA *private_key);
bool rsa_verify_signature(const char *msg, size_t msg_len, const uchar *hash, size_t hash_len, RSA *public_key);
RSA *generate_rsa_keys();
char *rsa_encrypt(const char *msg, RSA *public_key);
char *rsa_decrypt(const char *msg, RSA *private_key);
void export_public_key(const RSA *public_key, const std::string file_path = "./keys/", const std::string file_name = "public");
void export_private_key(const RSA *private_key, const std::string file_path = "./keys/", const std::string file_name = "private");
RSA *import_public_key(const std::string file_path = "./keys/", const std::string file_name = "public");
RSA *import_private_key(const std::string file_path = "./keys/", const std::string file_name = "private");
char *sign_message(const std::string msg, RSA *private_key);
bool verify_signature(const std::string msg, const char *signature, RSA *pulic_key);

#endif