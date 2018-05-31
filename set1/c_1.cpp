/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 1 - Convert hex to base64

 By: Guy Bar Yosef
*/

#include <iostream>
#include <string>
#include "baseClasses.hpp"

using namespace std;

int main() {
    string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    Hex hexString(input);
    cout << hexString.toBase64() << endl;
    return 0;
}