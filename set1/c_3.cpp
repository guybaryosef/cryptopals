/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 3 - Single-byte XOR cipher

 By: Guy Bar Yosef
 */

#include "functions.cpp" 
#include "baseClasses.hpp"

int main () {
    string input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    Hex hex1(input);

    message_score decrypted = singleXor(hex1.toBin());
    cout << "The message is: " << decrypted.message << endl;
    return 0;
}







