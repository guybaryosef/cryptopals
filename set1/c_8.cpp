/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 8 - Detect AES in ECB mode

 By: Guy Bar Yosef
 */

#include <iostream>

#include "functions.cpp"
#include "baseClasses.hpp"

int main() {
    string input = "c_8input.txt";
    cout << detectAESinECB(input) << endl;
    return 0;
}


