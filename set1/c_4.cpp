/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 4 - Detect single-character XOR

 By: Guy Bar Yosef
 */

#include "baseClasses.hpp" 
#include "functions.cpp"


int main() {
    string inputname = "c_4input.txt";

    message_score decrypted = detectxor(inputname);

    cout << "The message is: " << decrypted.message;
    cout << "The key is: " << decrypted.key << endl;
    return 0;
}



