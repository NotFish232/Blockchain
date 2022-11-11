#include "utils.hpp"
#include <iostream>

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

string Utils::sha256(const string &input) {
    uchar hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
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

string Utils::_sign_message(const string &msg, RSA *private_key) {
    EVP_PKEY *priKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, private_key);

    EVP_MD_CTX *m_RSASignCtx = EVP_MD_CTX_create();

    if (EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha256(), NULL, priKey) <= 0) {
        return NULL;
    }
    if (EVP_DigestSignUpdate(m_RSASignCtx, msg.c_str(), msg.length()) <= 0) {
        return NULL;
    }
    size_t length;
    if (EVP_DigestSignFinal(m_RSASignCtx, NULL, &length) <= 0) {
        return NULL;
    }
    string result(length, ' ');
    if (EVP_DigestSignFinal(m_RSASignCtx, (uchar *)result.c_str(), &length) <= 0) {
        return NULL;
    }
    EVP_MD_CTX_free(m_RSASignCtx);
    return result;
}

bool Utils::_verify_signature(const string &msg, const string &signature, RSA *rsa) {
    EVP_PKEY *pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX *m_RSAVerifyCtx = EVP_MD_CTX_create();
    if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL, pubKey) <= 0) {
        return false;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, msg.c_str(), msg.length()) <= 0) {
        return false;
    }
    int result = EVP_DigestVerifyFinal(m_RSAVerifyCtx, (uchar *)signature.c_str(), signature.length());
    EVP_MD_CTX_free(m_RSAVerifyCtx);
    return result == 1;
}

string Utils::base64_encode(const string &input) {
    const int pl = 4 * ((input.length() + 2) / 3);
    string output(pl, ' ');
    const int ol = EVP_EncodeBlock((uchar *)output.c_str(), (uchar *)input.c_str(), input.length());
    return pl == ol ? output : NULL;
}

string Utils::base64_decode(const string &input) {
    const int pl = 3 * input.length() / 4;
    string output(pl, ' ');
    const int ol = EVP_DecodeBlock((uchar *)output.c_str(), (uchar *)input.c_str(), input.length());
    if (pl != ol)
        return NULL;
    output.erase(std::find_if(output.rbegin(), output.rend(), [](const char &ch) {
        return ch != ' ' && ch != '\0';
    }).base(), output.end());
    return output;
}
string Utils::sign_message(const string &msg, RSA *private_key) {
    string enc_msg = Utils::_sign_message(msg, private_key);
    string base64_msg = Utils::base64_encode(enc_msg);
    return base64_msg;
}

bool Utils::verify_signature(const string &text, const string &signature, RSA *public_key) {
    string enc_msg = Utils::base64_decode(signature);
    return Utils::_verify_signature(text, enc_msg, public_key);
}

#pragma GCC diagnostic pop