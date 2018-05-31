/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 6 - Break repeating-key XOR

 By: Guy Bar Yosef
 */

#include <iostream>

//includes singleXor, englishScore, bintoString functions as well as #include directive to twoBinXor & has64toBin functions
#include "functions.cpp"
#include "baseClasses.hpp"

int main() {
    string input = "c_6input.txt";
    breakXor(input);
    return 0;
}
