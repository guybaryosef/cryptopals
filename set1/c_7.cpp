/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 7 - AES in ECB mode

 By: Guy Bar Yosef
 */

#include "functions.cpp"
#include "baseClasses.hpp"


int main(void) {
    string input = "c_7input.txt";
    unsigned char inputKey[] = "YELLOW SUBMARINE";
    
    ifstream input_file(input);
    assert (input_file && "Couldn't open file :(");

    Bin bininput;
    Base64 buf1;
    while (getline(input_file, buf1.val))
        bininput.val.append( buf1.toBin() );
    
    string temp = bininput.toString();
    unsigned char inputFile[temp.length()];
    for (int i = 0 ; i < temp.length() ; ++i)
        inputFile[i] = temp[i];

    unsigned char decrypted[temp.length()];
    decryptAES128inECB(inputFile, inputKey, decrypted, temp.length());
    
    ofstream outputFile("c_7ouput.txt");
    outputFile << decrypted << endl;

    return 0;
}


