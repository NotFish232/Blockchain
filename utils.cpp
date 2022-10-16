#include "utils.hpp"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
string Utils::sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
RSA *Utils::gen_rsa_keys() {
    size_t bits = 2048;
    size_t exponents = 65537;
    RSA *keypair = RSA_generate_key(bits, exponents, NULL, NULL);
    return RSA_check_key(keypair) == 1 ? keypair : NULL;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
char *Utils::rsa_encrypt(const char *msg, RSA *keypair) {
    uchar *out_buf = new uchar[RSA_size(keypair)];
    int result = RSA_public_encrypt(RSA_size(keypair), (uchar *)msg, out_buf, keypair, RSA_NO_PADDING);
    return result != -1 ? (char *)out_buf : NULL;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
char *Utils::rsa_decrypt(const char *msg, RSA *keypair) {
    uchar *out_buf = new uchar[RSA_size(keypair)];
    int result = RSA_private_decrypt(RSA_size(keypair), (uchar *)msg, out_buf, keypair, RSA_NO_PADDING);
    return result != -1 ? (char *)out_buf : NULL;
}
#pragma GCC diagnostic pop
