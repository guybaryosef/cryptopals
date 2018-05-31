/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 7 - AES in ECB mode

 By: Guy Bar Yosef
 */

#include <iostream>

#include "functions.cpp"
#include "baseClasses.hpp"


int main(void) {
    string input = "c_7input.txt";
    string key = "YELLOW SUBMARINE";
    try {
        decryptAES128inECB(input, key);
    }
    catch(string error) {
        cerr << error << endl;
        return 1;
    }
    return 0;
}


