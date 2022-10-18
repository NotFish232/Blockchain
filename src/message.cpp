#ifndef MESSAGE_H
#define MESSAGE_H
#include "crypto.hpp"

class Message {
private:
    RSA *sender_public_key;
    RSA *receiver_public_key;
    char *signature;
    char *message;

public:
    Message() {
    }
};
#endif