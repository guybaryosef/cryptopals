/*
 Cryptopals Crypto Challenges
 Set 2
 Functions used for Set 2

 By: Guy Bar Yosef
 */

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <openssl/aes.h>

using namespace std;

#include "../set1/functions.cpp"
#include "../set1/baseClasses.hpp"


#ifndef FUNCTIONS2
#define FUNCTIONS2


string implementPKSC7(string & input, int blocksize); // ipmlements PKSC#7 padding

void decryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length);
void encryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length);

string generateRandStr(int size);  // generates random string of specified size
unsigned char * encryptionOracle(string & in, string & mode, int & size); // encrypts and input 'in' with a random 16 key in either 128 bit AES ECB or CBC mode (with random IV)
string decryptionOracle(unsigned char *in, int size);


/*
  Implements PKSC#7 padding for an inputted string, based on the inputed blocksize.
  The padding works by finding the amount of bytes needed to make the input a multiple of the blocksize,
  and then adding that value that amount of times to the end of the string.
      EX (in hex): 03 03 03     (if 3 bytes are missing to complete the blocksize)
                   04 04 04 04  (if 4 bytes are missing to compelte the blocksize)
 */
string implementPKSC7( string & input, int blocksize ) {
    int paddingSize = blocksize - (input.length() % blocksize);
    if (paddingSize == blocksize)
        paddingSize = 0;
    
    // need to create the correct padding
    int size = to_string(blocksize).length();
    string pad = to_string(paddingSize);
    pad.insert(0, string(size-pad.length(), '0') );

    string padding;
    for (int i = 0 ; i < paddingSize ; ++i)
        padding.append(pad);

    string output = input + padding;
    return output;
}


/*
  Takes as input the initialization vector, a pointer to the input, encryption/decryption key (AES is symmetric so they are the same),
  and the length of the input (which should equal at least the length of the output).
  This function decrypts the inputted cyphertext and stores it in 'output'.
  Note that the initialization vector and input key should both be the same length.
 */
void decryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length) {
    AES_KEY key;
    AES_set_decrypt_key(inputkey, 128, &key);

    char temp[AES_BLOCK_SIZE];
    char result[AES_BLOCK_SIZE];
    char buffer[AES_BLOCK_SIZE];
    strncpy(buffer, IV, AES_BLOCK_SIZE);           // set up the initialization vector for the coming loop

    for (int i = 0 ; i < length ; i += AES_BLOCK_SIZE ) {
        AES_decrypt(input + i, (unsigned char*)temp, &key); //block cypher decryption, saving output in 'temp'
        
        for (int j = 0 ; j < AES_BLOCK_SIZE ; ++j)  // XOR 'temp' and 'buffer' (buffer is the previous ciphertext)
            result[j] = temp[j] ^ buffer[j];

        strncpy((char *) (output + i), result, AES_BLOCK_SIZE); // copy into output the resulting XORed plaintext
        
        strncpy(buffer, (const char *)(input + i), AES_BLOCK_SIZE); // prepare 'buffer' for next iteration of loop
    }
}


/*
  Takes as input an initialization vector, pointer to an input, encryption/decryption key (AES is symmetric so they are the same),
  and the length of the input (which should equal at least the length of the output).
  This function encrypts the inputted plaintext and stores it in 'output'.
  Note that the initialization vector and input key should both be the same length.
 */
void encryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length) {
    AES_KEY key;
    AES_set_encrypt_key(inputkey, 128, &key);

    char temp[AES_BLOCK_SIZE];
    char buffer[AES_BLOCK_SIZE];
    char result[AES_BLOCK_SIZE];

    strncpy(temp, IV, AES_BLOCK_SIZE);   // insert the initialization vector so that it is set up for the coming loop
    for (int i = 0 ; i < length ; i += AES_BLOCK_SIZE ) {
        strncpy(buffer, (char *)(input + i), AES_BLOCK_SIZE); 

        for (int j = 0 ; j < AES_BLOCK_SIZE ; ++j) // XOR plaintext and previous cyphertext
            result[j] = temp[j] ^ buffer[j];

        // split the following into two parts so that 'temp' can be used for next iteration's XOR operation
        AES_encrypt((const unsigned char *)result , (unsigned char *)temp, &key);
        strncpy( (char *)(output + i), temp, AES_BLOCK_SIZE);
    }
}


/* 
  Generates a (uniformly) random string of size 'size'.
*/
string generateRandStr(int size) {
    srand (time(NULL));
    string output;
    for (int i = 0 ; i < size ; ++i )
        output.push_back( (char)(abs(rand() % 128)) );
    return output;
}


/*
  Encryption oracle, i.e. encrypts some input with an unknown key and an unknown amount of padding in either ECB or CBC mode.
  Although the mode is chosen at random, the oracle will update the input 'mode' according to its choice.
*/
unsigned char *encryptionOracle(string & in, string & mode, int & size) {
    // generating random key
    string buf = generateRandStr(AES_BLOCK_SIZE);
    unsigned char key[AES_BLOCK_SIZE + 1];
    strncpy((char *)key, buf.c_str(), AES_BLOCK_SIZE);
    key[16] = '\0';

    //adding 5-10 random bytes before and after input string
    
    int num = 5 + (rand() % 6);
    string beg = generateRandStr(num);
    num = 5 + (rand() % 6);
    string end = generateRandStr(num);
    in = beg + in + end;
    

    // pad the input accordingly and convert to unsigned char type (needed for encryption)
    in = implementPKSC7(in, AES_BLOCK_SIZE);

    cout << in << endl;
    unsigned char *output = new unsigned char[in.length() + 1];
    unsigned char input[in.length() + 1];
    size = in.length();
    for (int i = 0 ; i < in.length() ; ++i) 
        input[i] = in[i];
    // randomly choose between ECB or CBC mode, and if in CBC mode then also input a random initialization vector
    srand (time(NULL));
    if ( rand() % 2) {
        mode = "ECB";
        encryptAES128inECB(input, key, output, in.length());
    }
    else {
        mode = "CBC";
        string buf2 = generateRandStr(AES_BLOCK_SIZE);
        char IV[17];
        strncpy(IV, buf2.c_str(), AES_BLOCK_SIZE);

        encryptAES128inCBC(IV, input, key, output, in.length());
    }
    return output;
}


/*
  Given an input 'in', this function will return whether the input was encrypted with ECB or CBC mode.
  We will find the average hamming distance between consecutive 16 byte blocks, and depending on some threshold
  we will determine if this was likely an ECB encrypted text (if it passes the threshold) or CBC encrypted. 
*/
string decryptionOracle(unsigned char *in, int size) {
    string mode;
    double totalhammingDistance = 0;
    
    char cur[AES_BLOCK_SIZE + 1];
    char prev[AES_BLOCK_SIZE+ 1];

    prev[16] = '\0';
    cur[16] = '\0';

    //find the edit distance between each two blocks in the input, looking for a pair with a distance less than threshold.
    for (int i = 0 ; i < size ; i += AES_BLOCK_SIZE) {
        strncpy(prev, (char *)(in+i), AES_BLOCK_SIZE);
        Bin bin1(string(prev), "string");

        for (int j = i + AES_BLOCK_SIZE ; j < size ; j += AES_BLOCK_SIZE) {
            strncpy(cur, (char *)(in + j), AES_BLOCK_SIZE);
            Bin bin2(string(cur), "string");
            
            cout << editDistance(bin1.val, bin2.val) << " - ";
            totalhammingDistance += editDistance(bin1.val, bin2.val);
        }
        cout << endl;
    }
    int avg = totalhammingDistance / (size/AES_BLOCK_SIZE);
    cout << "Avg: " << avg << endl;
    if ( avg < 60)
        return "ECB";
    else
        return "CBC";
}


#endif