/*
 Cryptopals Crypto Challenges
 Set 2
 Challenge 9 - Implement PKCS#7 padding

 By: Guy Bar Yosef
 */

#include "functions2.cpp"

int main () {
    string plaintext = "YELLOW SUBMARINE";
    int block = 20;
    string cyphertext = implementPKSC7(plaintext, block);
    cout << cyphertext << endl;
    return 0;
}




