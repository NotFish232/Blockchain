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

RSA *Utils::generate_rsa_keys() {
    size_t bits = 2048;
    size_t exponents = 65537;
    RSA *keypair = RSA_generate_key(bits, exponents, NULL, NULL);
    return RSA_check_key(keypair) == 1 ? keypair : NULL;
}

char *Utils::rsa_encrypt(const char *msg, RSA *keypair) {
    uchar *out_buf = new uchar[RSA_size(keypair)];
    int result = RSA_public_encrypt(RSA_size(keypair), (uchar *)msg, out_buf, keypair, RSA_NO_PADDING);
    return result != -1 ? (char *)out_buf : NULL;
}

char *Utils::rsa_decrypt(const char *msg, RSA *keypair) {
    uchar *out_buf = new uchar[RSA_size(keypair)];
    int result = RSA_private_decrypt(RSA_size(keypair), (uchar *)msg, out_buf, keypair, RSA_NO_PADDING);
    return result != -1 ? (char *)out_buf : NULL;
}

void Utils::export_public_key(const RSA *keypair, const string file_path) {
    BIO *file = BIO_new_file((file_path + "public.pem").c_str(), "w+");
    PEM_write_bio_RSAPublicKey(file, keypair);
    BIO_free(file);
}

void Utils::export_private_key(const RSA *keypair, const string file_path) {
    BIO *file = BIO_new_file((file_path + "private.pem").c_str(), "w+");
    PEM_write_bio_RSAPrivateKey(file, keypair, NULL, NULL, 0, NULL, NULL);
    BIO_free(file);
}

RSA *Utils::import_public_key(const string file_path) {
    RSA *keypair = NULL;
    BIO *file = BIO_new_file((file_path + "public.pem").c_str(), "rt");
    PEM_read_bio_RSAPublicKey(file, &keypair, NULL, NULL);
    return keypair;
}

RSA *Utils::import_private_key(const string file_path) {
    RSA *keypair = NULL;
    BIO *file = BIO_new_file((file_path + "private.pem").c_str(), "rt");
    PEM_read_bio_RSAPrivateKey(file, &keypair, NULL, NULL);
    return keypair;
}
#pragma GCC diagnostic pop