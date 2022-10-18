#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>
#include <string>
#include <iomanip>

std::string sha256_hash(const std::string str);

#endif