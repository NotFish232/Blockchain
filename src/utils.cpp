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
uchar *Utils::_sign_message(const uchar *msg, size_t msg_len, size_t *enc_msg_len_ptr, RSA *rsa) {
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

bool Utils::_verify_signature(const string &msg, const uchar *hash, size_t hash_len, RSA *rsa) {
    EVP_PKEY *pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX *m_RSAVerifyCtx = EVP_MD_CTX_create();

    if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL, pubKey) <= 0) {
        return false;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, msg.c_str(), msg.length()) <= 0) {
        return false;
    }
    int result = EVP_DigestVerifyFinal(m_RSAVerifyCtx, hash, hash_len);
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return result == 1;
}
string Utils::base64_encode(const uchar *buffer, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *buffer_ptr;

    bio= BIO_new(BIO_s_mem());
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_write(b64, buffer, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &buffer_ptr);

    string result(buffer_ptr->data, buffer_ptr->length);
    BIO_free_all(bio);

    return result;
}

size_t Utils::calculate_base64_length(const char *b64_input) {
    size_t len = strlen(b64_input), padding = 0;
    if (b64_input[len - 1] == '=') {
        padding = b64_input[len - 2] == '=' ? 2 : 1;
    }
    return (3 * len) / 4 - padding;
}

uchar *Utils::base64_decode(const string &input, size_t *length_ptr) {
    BIO *bio, *b64;

    int decode_len = Utils::calculate_base64_length(input.c_str());
    uchar *buffer = new uchar[decode_len + 1];
    buffer[decode_len] = '\0';

    bio = BIO_new_mem_buf(input.c_str(), -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    *length_ptr = BIO_read(bio, buffer, input.length());
    BIO_free_all(bio);
    return buffer;
}

string Utils::sign_message(const string &msg, RSA *private_key) {
    size_t enc_msg_len;
    uchar *enc_msg = Utils::_sign_message((uchar *)msg.c_str(), msg.length(), &enc_msg_len, private_key);
    string base64_msg = Utils::base64_encode(enc_msg, enc_msg_len);
    delete enc_msg;
    return base64_msg;
}

bool Utils::verify_signature(const string &text, const string &signature, RSA *public_key) {
    size_t enc_msg_len;
    uchar *enc_msg = Utils::base64_decode(signature, &enc_msg_len);
    bool result = Utils::_verify_signature(text, enc_msg, enc_msg_len, public_key);
    delete enc_msg;
    return result;
}

#pragma GCC diagnostic pop