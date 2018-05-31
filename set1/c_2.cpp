/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 2 - Fixed XOR

 By: Guy Bar Yosef
 */

#include <iostream>
#include <string>
#include <cmath>

#include "functions.cpp"
#include "baseClasses.hpp"

using namespace std;

int main() {
    string input1 = "1c0111001f010100061a024b53535009181c";
    string input2 = "686974207468652062756c6c277320657965";
    Hex hex1(input1);
    Hex hex2(input2);
    Bin bin1( twoBinXor( hex1.toBin(),hex2.toBin() ) );
    cout << bin1.toHex() << endl;
    return 0;
}


