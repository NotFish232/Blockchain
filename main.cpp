#include "src/network.hpp"
#include "src/crypto.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
   /* RSA *private_key = Crypto::import_private_key();
    RSA *public_key = Crypto::import_public_key();
    string msg = "*)$#*)($#*843908093280943809FJjsdlkjflkjKLFDJLK$#*()*)(#4093FJLKD random message $*)*#)*()4890380943820";
    string sig = Crypto::sign_message(msg, private_key);
    string enc = Crypto::base64_encode(sig);
    string dec = Crypto::base64_decode(enc);
    cout << endl;
    for (const char &ch: sig) cout << "`" << ch << "` ";
    cout << "\n\n\n\n break \n\n\n\n";
    for (const char &ch: dec) cout << "`" << ch << "` ";
    cout << endl;
    cout << boolalpha << (sig == dec) << endl;*/
    RSA *private_key = Crypto::import_private_key();
    RSA *public_key = Crypto::import_public_key();
    string msg = "*)$#*)($#*843908093280943809FJjsdlkjflkjKLFDJLK$#*()*)(#4093FJLKD random message $*)*#)*()4890380943820 ";
    /*for (int i = msg.length() - 1; i >= 0; --i) {
        string sub = msg.substr(i);
        string signature = Crypto::sign_message(sub, private_key);
       //  cout << signature << endl;
        bool result = Crypto::verify_signature(sub, signature, public_key);
        cout << boolalpha << result << endl;
    }*/
    string signature = Crypto::sign_message(msg, private_key);
    bool result = Crypto::verify_signature(msg, signature, public_key);
    cout << boolalpha << result << endl;
    Crypto::free(private_key);
    Crypto::free(public_key);
    return 0;
}