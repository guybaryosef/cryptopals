/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 7 - AES in ECB mode

 By: Guy Bar Yosef
 */

#include "functions.cpp"
#include "../set2/functions2.cpp"
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
    unsigned char inputFile[temp.length() + 1];

    for (int i = 0 ; i < temp.length() ; ++i)
        inputFile[i] = temp[i];

    inputFile[temp.length()] = '\0';

    unsigned char decrypted[temp.length() + 1];
    decryptAES128inECB(inputFile, inputKey, decrypted, temp.length());
    decrypted[temp.length()] = '\0';

    ofstream output_file("c_7output.txt");
    output_file << decrypted << endl;

    input_file.close();
    output_file.close();

    ///////////// TESTING encryption and decryption in ECB //////////////

    input_file.open("c_7input2.txt");
    assert (input_file && "Couldn't open file :(");

    string buffer;
    string total;
    while (getline(input_file, buffer))
        total.append( buffer );
    
    temp = total;
    temp = implementPKSC7(temp, AES_BLOCK_SIZE); // pad the plaintext before encryption

    unsigned char inputFile2[temp.length() + 1];

    for (int i = 0 ; i < temp.length() ; ++i)
        inputFile2[i] = temp[i];

    inputFile2[temp.length()] = '\0';

    unsigned char encrypted[temp.length() + 1];
    encrypted[temp.length()] = '\0';

    encryptAES128inECB(inputFile2, inputKey, encrypted, temp.length());

    unsigned char decrypted2[temp.length() + 1];
    decrypted2[temp.length()] = '\0';

    decryptAES128inECB(encrypted, inputKey, decrypted2, temp.length());

    output_file.open("c_7output2.txt");
    output_file << decrypted2 << endl;

    input_file.close();
    output_file.close();


    return 0;
}


