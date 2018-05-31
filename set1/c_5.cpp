/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 5 - Repeating-key XOR

 By: Guy Bar Yosef
 */

#include <iostream>

#include "functions.cpp"
#include "baseClasses.hpp"


int main() {
    string input = "c_5input.txt";
    string key = "ICE";
    Bin bin1( repeatingxor(input, key));
    cout << bin1.toHex() << endl;  //printed as binary digit string
    
    return 0;
}
