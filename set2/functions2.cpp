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


string implementPKSC7(string & input, int blocksize); // ipmlements PKSC#7 padding
void decryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length);
void encryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length);


/*
 *  Implements PKSC#7 padding for an inputted string, based on the inputed blocksize.
 *  The padding works by finding the amount of bytes needed to make the input a multiple of the blocksize,
 *  and then adding that value that amount of times to the end of the string.
 *      EX (in hex): 03 03 03
 *                   04 04 04 04 
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
  Takes as input a pointer to an input, encryption/decryption key (AES is symmetric so they are the same),
  and the length of the input (which should equal at least the length of the output).
  This function decrypts the inputted cyphertext and stores it in 'output'.
 */
void decryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length) {
    AES_KEY key;
    AES_set_decrypt_key(inputkey, 128, &key);

    char temp[AES_BLOCK_SIZE];
    char result[AES_BLOCK_SIZE];
    char buffer[AES_BLOCK_SIZE];
    strcpy(buffer, IV);           // set up the initialization vector for the coming loop

    for (int i = 0 ; i < length ; i += AES_BLOCK_SIZE ) {
        AES_decrypt(input + i, (unsigned char*)temp, &key); //block cypher decryption, saving output in 'temp'
        
        for (int j = 0 ; j < AES_BLOCK_SIZE ; ++j)  // XOR 'temp' and 'buffer' (buffer is the previous ciphertext)
            result[j] = temp[j] ^ buffer[j];

        strncpy((char *) (output + i), result, AES_BLOCK_SIZE); // copy into output the resulting XORed plaintext
        
        strncpy(buffer, (const char *)input + i, AES_BLOCK_SIZE); // prepare 'buffer' for next iteration of loop
    }
}


/*
  Takes as input a pointer to an input, encryption/decryption key (AES is symmetric so they are the same),
  and the length of the input (which should equal at least the length of the output).
  This function encrypts the inputted plaintext and stores it in 'output'.
 */
void encryptAES128inCBC(char *IV, unsigned char *input, unsigned char inputkey[], unsigned char *output, int length) {
    AES_KEY key;
    AES_set_encrypt_key(inputkey, 128, &key);

    char temp[AES_BLOCK_SIZE];
    char buffer[AES_BLOCK_SIZE];
    char result[AES_BLOCK_SIZE];

    strcpy(temp, IV);   // insert the initialization vector so that it is set up for the coming loop
    for (int i = 0 ; i < length ; i += AES_BLOCK_SIZE ) {
        strncpy(buffer, (char *)(input + i), AES_BLOCK_SIZE); //not necessary, simply to organize the loop more nicely

        for (int j = 0 ; j < AES_BLOCK_SIZE ; ++j) // XOR plaintext and previous cyphertext
            result[j] = temp[j] ^ buffer[j];

        // split the following into two parts so that 'temp' can be used for next iteration's XOR operation
        AES_encrypt((const unsigned char *)result , (unsigned char *)temp, &key);
        strncpy( (char *)(output + i), temp, AES_BLOCK_SIZE);
    }
}
