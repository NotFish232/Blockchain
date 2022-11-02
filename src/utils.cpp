#include "utils.hpp"
#include <iostream>

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

string Utils::sha256(const string &str) {
    uchar hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.length());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

void Utils::free(RSA *keypair) {
    RSA_free(keypair);
}

RSA *Utils::generate_rsa_keys() {
    size_t bits = 2048;
    size_t exponents = 65537;
    RSA *keypair = RSA_generate_key(bits, exponents, NULL, NULL);
    return RSA_check_key(keypair) == 1 ? keypair : NULL;
}

char *Utils::rsa_encrypt(const char *msg, RSA *public_key) {
    uchar *out_buf = new uchar[RSA_size(public_key)];
    int result = RSA_public_encrypt(RSA_size(public_key), (uchar *)msg, out_buf, public_key, RSA_NO_PADDING);
    return result != -1 ? (char *)out_buf : NULL;
}

char *Utils::rsa_decrypt(const char *msg, RSA *private_key) {
    uchar *out_buf = new uchar[RSA_size(private_key)];
    int result = RSA_private_decrypt(RSA_size(private_key), (uchar *)msg, out_buf, private_key, RSA_NO_PADDING);
    return result != -1 ? (char *)out_buf : NULL;
}

int Utils::export_public_key(const RSA *public_key, const string file_path, const string file_name) {
    BIO *file = BIO_new_file((file_path + file_name + ".pem").c_str(), "w+");
    int result = PEM_write_bio_RSAPublicKey(file, public_key);
    BIO_free(file);
    return result;
}

int Utils::export_private_key(const RSA *private_key, const string file_path, const string file_name) {
    BIO *file = BIO_new_file((file_path + file_name + ".pem").c_str(), "w+");
    int result = PEM_write_bio_RSAPrivateKey(file, private_key, NULL, NULL, 0, NULL, NULL);
    BIO_free(file);
    return result;
}

RSA *Utils::import_public_key(const string file_path, const string file_name) {
    RSA *public_key = NULL;
    BIO *file = BIO_new_file((file_path + file_name + ".pem").c_str(), "rt");
    PEM_read_bio_RSAPublicKey(file, &public_key, NULL, NULL);
    BIO_free(file);
    return public_key;
}

RSA *Utils::import_private_key(const string file_path, const string file_name) {
    RSA *private_key = NULL;
    BIO *file = BIO_new_file((file_path + file_name + ".pem").c_str(), "rt");
    PEM_read_bio_RSAPrivateKey(file, &private_key, NULL, NULL);
    BIO_free(file);
    return private_key;
}
uchar *Utils::rsa_sign(const uchar *msg, size_t msg_len, size_t *enc_msg_len_ptr, RSA *rsa) {
    EVP_MD_CTX *m_RSASignCtx = EVP_MD_CTX_create();
    EVP_PKEY *priKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, rsa);
    if (EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha256(), NULL, priKey) <= 0) {
        return NULL;
    }
    if (EVP_DigestSignUpdate(m_RSASignCtx, msg, msg_len) <= 0) {
        return NULL;
    }
    if (EVP_DigestSignFinal(m_RSASignCtx, NULL, enc_msg_len_ptr) <= 0) {
        return NULL;
    }
    uchar *enc_msg = new uchar[*enc_msg_len_ptr];
    if (EVP_DigestSignFinal(m_RSASignCtx, enc_msg, enc_msg_len_ptr) <= 0) {
        return NULL;
    }
    EVP_MD_CTX_free(m_RSASignCtx);
    return enc_msg;
}

bool Utils::rsa_verify_signature(const char *msg, size_t msg_len, const uchar *hash, size_t hash_len, RSA *rsa) {
    EVP_PKEY *pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX *m_RSAVerifyCtx = EVP_MD_CTX_create();

    if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL, pubKey) <= 0) {
        return false;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, msg, msg_len) <= 0) {
        return false;
    }
    int auth_status = EVP_DigestVerifyFinal(m_RSAVerifyCtx, hash, hash_len);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return auth_status == 1;
}

string Utils::base64_encode(const string &input) {
    const auto base64_memory = BIO_new(BIO_s_mem());
    auto base64 = BIO_new(BIO_f_base64());
    base64 = BIO_push(base64, base64_memory);
    BIO_write(base64, input.c_str(), input.length());
    BIO_flush(base64);
    BUF_MEM *buffer_memory;
    BIO_get_mem_ptr(base64, &buffer_memory);
    string base64_encoded = string(buffer_memory->data, buffer_memory->length - 1);
    BIO_free_all(base64);
    return base64_encoded;
}

string Utils::base64_decode(const string &input) {
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO *source = BIO_new_mem_buf(input.c_str(), -1);
    BIO_push(b64, source);
    const int maxlen = input.length() / 4 * 3 + 1;
    string d(maxlen, ' ');
    const int len = BIO_read(b64, (void *)d.data(), maxlen);
    BIO_free(b64);
    BIO_free(source);
    return d;
}

char *Utils::sign_message(const std::string msg, RSA *private_key) {
    /*size_t enc_msg_len;
    uchar *enc_msg = Utils::rsa_sign((uchar *)msg.c_str(), msg.length(), &enc_msg_len, private_key);
    char *base64_msg = Utils::base64_encode(enc_msg, enc_msg_len);
    delete enc_msg;
    return base64_msg;*/
}

bool Utils::verify_signature(const std::string text, const char *signature, RSA *public_key) {
    /*size_t enc_msg_len;
    uchar *enc_msg = Utils::base64_decode(signature, &enc_msg_len);
    bool result = Utils::rsa_verify_signature(text.c_str(), text.length(), enc_msg, enc_msg_len, public_key);
    delete enc_msg;
    return result;*/
}

#pragma GCC diagnostic pop