#include "src/network.hpp"
#include "src/utils.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
   /* RSA *private_key = Utils::import_private_key();
    RSA *public_key = Utils::import_public_key();
    string msg = "*)$#*)($#*843908093280943809FJjsdlkjflkjKLFDJLK$#*()*)(#4093FJLKD random message $*)*#)*()4890380943820";
    string sig = Utils::sign_message(msg, private_key);
    string enc = Utils::base64_encode(sig);
    string dec = Utils::base64_decode(enc);
    cout << endl;
    for (const char &ch: sig) cout << "`" << ch << "` ";
    cout << "\n\n\n\n break \n\n\n\n";
    for (const char &ch: dec) cout << "`" << ch << "` ";
    cout << endl;
    cout << boolalpha << (sig == dec) << endl;*/
    RSA *private_key = Utils::import_private_key();
    RSA *public_key = Utils::import_public_key();
    string msg = "*)$#*)($#*843908093280943809FJjsdlkjflkjKLFDJLK$#*()*)(#4093FJLKD random message $*)*#)*()4890380943820 ";
    for (int i = msg.length() - 1; i >= 0; --i) {
        string sub = msg.substr(i);
        string signature = Utils::sign_message(sub, private_key);
       //  cout << signature << endl;
        bool result = Utils::verify_signature(sub, signature, public_key);
        cout << boolalpha << result << endl;
    }
    return 0;
}