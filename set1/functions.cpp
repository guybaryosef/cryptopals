/*
 Cryptopals Crypto Challenges
 Set 1
 Functions used for Set 1

 By: Guy Bar Yosef
 */

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <map>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <vector>
#include <openssl/aes.h>

#include "baseClasses.hpp"

using namespace std;

#ifndef FUNCTIONS
#define FUNCTIONS

#define KEY_SIZE 16  //16 *sizeof(unsigned char) = 16 * 8 = 128 = mode of AES we are using


// each decrypted message has a number ('score') associated with it, representing how likely it is to be english
typedef struct message_score {
    double score = 0;   // english score associated with decrypted message
    char key;           // character message was XORed against
    string message;     // decrypted message
} message_score;

// Struct to assist with finiding the length of the encryption key (Used in step 1. of the breakXOR function)
typedef struct keyDist {
    double dist;        // edit distance of potitional key length
    int keylen;         // potential key length
} keyDist;


string twoBinXor(string bin1, string bin2);  //XORs two binary strings

message_score singleXor(string bininput);    // takes a binary input and returns a struct with the decrypted string and key
double englishScore(string input);           // scores text, higher score relates to higher chance to be english

message_score detectxor(string filename);    // finds the single-xor encrypted message from a file full of encrypted messages

string repeatingxor(string inputfile, string key);   //encrypts a string using repeated-key XOR with key

double editDistance(string one, string two); // finds the number of differing bits between two binary strings
void breakXor(string input_file);            // function to break the Vigenere Cypher

// 128 bit AES in ECB mode
void decryptAES128inECB(unsigned char *input, unsigned char inputkey[], unsigned char *output, int length);  
void encryptAES128inECB(unsigned char *input, unsigned char inputkey[], unsigned char *output, int length);  
string detectAESinECB(string file);    //detects the 128bit AES in ECB mode encryption of a file.

/*
 *  takes two strings of binary numbers and outputs their XOR combination as a string
 */
string twoBinXor(string bin1, string bin2) {
    
    if (bin1.length() != bin2.length()) {  
        
        // buffers the shorter input with '0's
        int diff = abs((int)bin1.length() - (int)bin2.length() );

        if (bin1.length() > bin2.length())
            bin2.insert(0, diff, '0');
        else
            bin1.insert(0, diff, '0');
    }

    string output(bin1.length(),0);
    for (int i = 0 ; i < bin1.length() ; ++i)   //XORs each bit
        output[i] = (bin1[i] == bin2[i]) ? '0':'1';
    
    return output;
}


/*
 * Takes binary input and returns a string that was encrypted through an XOR with a single ascii char
 * Main idea here is that: [ message XOR key XOR key = message ], and our input was [message XOR key], the encrypted message
 */
 message_score singleXor(string bininput) {
    message_score output;      //output message and its associated english score

    //iterate through all ascii values, finding the one which produces the heighest english score, signifying how close the potential message is in english
    for (unsigned char c = 0 ; c < 128 ; c++) {
        bitset<8> bytekey(c);   //convert each ascii char to its 8 bit binary value, to use in XOR with encrypted message
        string binkey = bytekey.to_string();

        //'possiblekey' will be a string the length of the decrypted message, populated with one repeating char
        string possiblekey;
        while (possiblekey.length() < bininput.length())
            possiblekey.append(binkey); 

        Bin temp (twoBinXor(possiblekey, bininput));
        string message = temp.toString();
        double score = englishScore(message);

        if (score > output.score) {  // if we have a new high-scored message, make that the new decrypted message
            output.score = score;
            output.message = message;
            output.key = c;
        }
    }
    return output;
 }


/*
 *  scores text, higher score relates to higher chance to be english
 */
double englishScore(string input) {
    // letter weights derived from https://upload.wikimedia.org/wikipedia/commons/d/d5/English_letter_frequency_%28alphabetic%29.svg
    map<char,double> scorecard;
        scorecard['a'] = 0.082;  scorecard['b'] = 0.016;  scorecard['c'] = 0.028;  scorecard['d'] = 0.043;  
        scorecard['e'] = 0.128;  scorecard['f'] = 0.022;  scorecard['g'] = 0.020;  scorecard['h'] = 0.061;
        scorecard['i'] = 0.071;  scorecard['j'] = 0.003;  scorecard['k'] = 0.006;  scorecard['l'] = 0.039;
        scorecard['m'] = 0.023;  scorecard['n'] = 0.070;  scorecard['o'] = 0.076;  scorecard['p'] = 0.019;  
        scorecard['q'] = 0.002;  scorecard['r'] = 0.061;  scorecard['s'] = 0.062;  scorecard['t'] = 0.091;
        scorecard['u'] = 0.028;  scorecard['v'] = 0.009;  scorecard['w'] = 0.023;  scorecard['x'] = 0.003;  
        scorecard['y'] = 0.019;  scorecard['z'] = 0.001;
        //assign to numbers equivalent score, except 0, which is slightly more popular (arbitrarily decided)
        scorecard['0'] = 0.010;  scorecard['1'] = 0.008;  scorecard['2'] = 0.008;  scorecard['3'] = 0.008;
        scorecard['4'] = 0.008;  scorecard['5'] = 0.008;  scorecard['6'] = 0.008;  scorecard['7'] = 0.008;
        scorecard['8'] = 0.008;  scorecard['9'] = 0.008;  scorecard[' '] = 0.068;

    double score = 0;
    transform( input.begin(), input.end(), input.begin(), ::tolower );  // converts input to lowercase
    for (auto it = input.begin() ; it != input.end() ; ++it )           // iterate through string, summing up its letter scores
        score += scorecard[ *it ];
    
    return score;
}


/*
 Takes in a file name and path which contains any amount of hex encoded strings among which one was encrypted with a single-character XOR.
 This function finds the encrypted message by comparing the heighest-english-scored message of each hex encoded string and returning the heightest score of them all.
 */
message_score detectxor(string filename) {
    
    ifstream input_file (filename);
    assert( input_file.is_open() && "File could not be found :(" );    // stops program if file doesn't or couldn't be found

    message_score decrypted;   // the output of this function, the decrypted message struct

    string hexcandidate;
    input_file >> hexcandidate;

    while( !input_file.eof() ) {  // iterates through the hex-encoded strings inside the input file
        Hex hex1(hexcandidate);
        message_score candidate = singleXor(hex1.toBin());  // returns the heighest-english-scored key and message of the hex encoded candidate
        if (candidate.score > decrypted.score)              // compares with the heighest scored message up until now in file
            decrypted = candidate;
        input_file >> hexcandidate;
    }
    return decrypted;
}


/*
  Takes a text file as input and returns the contents of that file XORed repeatedly with the key "ICE".
 */
string repeatingxor(string inputfile, string key) {
    ifstream input_file(inputfile);     // this constructor requires C++11 or heigher to compile
    assert( input_file.is_open() && "File could not be found :(" );

    string encrypted;  // our output
    int count = 0;
    char c;
    input_file >> noskipws >> c;    //reads char by char not skipping whitespace
    while ( !input_file.eof() ) {
        //convert the chars to their respective string of bytes and then XORs them together
        bitset<8> bytekey(key[count % key.length()]);   //cycles through the chars of 'key'
        bitset<8> bytec(c);
        encrypted.append(twoBinXor( bytekey.to_string(), bytec.to_string() ));
        count++;
        input_file >> noskipws >> c;
    }
    return encrypted;
}


/*
  Funtion that breaks the Vigenere Cypher.
  Takes in as inputpath & name of a file containing a base64'ed repeated-XOR encrypted text.
  Returns void, yet prints out the key and the decrypted message in file 'c6_output.txt'.
  Steps that this function follows:
    1. Find the 3 most probable lengths of the repeated-XOR key using the edit distance of consecutive key lengths.
    2. Goes through input file 3 times, each time splitting it up into both blocks of the probable keylength size as well as blocks of single char of key repeated over and over.
            - EX: if keylength was 2 and we had input 'abcdefg', will split into both blocks: ['ab', 'cd', 'ef', 'g'] as well as ['aceg', 'bdf'].
    3. Conducts a singleXor to each of the single char blocks to find the likely char of key.
    4. Finds the probability of each key's decrypted message's probability of being english, and writes that message and respective key into the output file 'c6_output.txt'.
 */
void breakXor(string input) {
    ifstream input_file(input);
    assert (input_file && "Couldn't open file :(");

    // Step 1.)
    // read in enough of the beginning of the input file to to compute consecutive edit distance of key lengths of up to 40 bytes
    string begn, begn_buffer;
    Base64 buf1;
    for (int i = 0 ; i < 8 ; i++) {
        getline(input_file, buf1.val);
        begn.append( buf1.toBin() );
    }

    vector <keyDist> min(3, {1, 0});    // This vector will have the 3 most likely keylengths after the following for-loop.
    for (int KEYLEN = 2 ; KEYLEN < 41 ; KEYLEN++) {
        // 'distance' will contain the edit length of 3 consecutive key length comparisons
        double distance = editDistance( begn.substr( 0 , KEYLEN*8),
                                        begn.substr(1*KEYLEN*8, KEYLEN*8) );
        distance += editDistance( begn.substr(1*KEYLEN*8, KEYLEN*8),
                                  begn.substr(2*KEYLEN*8, KEYLEN*8) );
        distance += editDistance( begn.substr(2*KEYLEN*8, KEYLEN*8),
                                  begn.substr(3*KEYLEN*8, KEYLEN*8) );

        keyDist temp = {distance/(KEYLEN*8*3) , KEYLEN};
        for (int k = 0 ; k < 3 ; k++) {
            if (temp.dist < min[k].dist) {
                min[k] = temp;
                break;
            }
        }
    }

    // Step 2.)
    string message; // decrypted, original message
    double messagescore = 0;    // the messages englishScore value (how likely it is to be english);
    string key;     // the correct key used to encrypt message

    for (int i = 0 ; i < 3 ; i++) {
        input_file.clear();
        input_file.seekg(0, input_file.beg);    //sets char index of input_file to the beginning of the file

        vector <string> keyBlocks;
        vector <string> byteBlocks(min[i].keylen);
        string buffer, buffer_helper;

        while ( getline(input_file, buf1.val) )    // reads in entire input file into 'buffer' string.
            buffer.append( buf1.toBin() );

        while ( buffer.length() >= min[i].keylen*8 ) {   // splits the buffer string into the two types of blocks described in function description.
            for (int j = 0 ; j < min[i].keylen ; j++)
                byteBlocks[j].append(buffer.substr(0+8*j, 8));
            keyBlocks.push_back(buffer.substr(0, min[i].keylen*8) );
            buffer = buffer.substr(min[i].keylen*8);
        }
        // Necessary to deal with the end of the message in the case that the message does not devide into keylength equal parts. Although it would be more correct to add this last part into the single char blocks, it most likely will not make a difference in the singleXor calculation and so ommmited for the sake of time.
        if (buffer.length() > 0)
            keyBlocks.push_back(buffer);

        // Step 3.)
        message_score tempkey;   // temporary var used for the consecutive singleXor-ing of all the key's chars.
        string possibleKey;      // the most likely key ]for this specific keylength.
        string binPossibleKey;   // the binary digit string of th possibleKey.
        for (int k = 0 ; k < min[i].keylen ; k++) {
            tempkey = singleXor(byteBlocks[k]);

            possibleKey.push_back(tempkey.key);

            bitset <8> temp(possibleKey[k]);
            binPossibleKey.append( temp.to_string() );
        }

        string possiblemessage; // the message resulting from the decryption with the 'possibleKey'.
        Bin buf2;
        for (int k = 0 ; k < keyBlocks.size() ; k++ ) {
            buf2.val = twoBinXor(keyBlocks[k], binPossibleKey);
            possiblemessage.append(buf2.toString() );
        
        }

        // Step 4.)
        if (englishScore(possiblemessage) > messagescore) {
            messagescore = englishScore(possiblemessage);
            message = possiblemessage;
            key = possibleKey;
        }
    }
    // The writing of the correct key and message to output file 'c6_output.txt'.
    ofstream output_file("c_6output.txt");
    output_file << "Key: " << key << '\n' << "Message: " << endl;
    output_file << message << endl;
}


/*
  This funciton computes the Hamming Distance between two binary strings.
  Input is the two strings to compare and the output is the number of differing bits between the two.
    - Note: This function assumes correct input, meaning that it does not check that the two inputed binary strings are equal length.
 */
double editDistance(string bin1, string bin2) {
    double count = 0;
    for (int i = 0 ; i < bin1.length() ; i++)
        if (bin1[i] != bin2[i])
            count++;
    return count;
}


/*
  Takes as input a pointer to an input, encryption/decryption key (AES is symmetric so they are the same),
  and the length of the input (which should equal at least the length of the output).
  This function decrypts the input and stores it in 'output'.
 */
void decryptAES128inECB(unsigned char *input, unsigned char inputkey[], unsigned char *output, int length) {
    AES_KEY key;
    AES_set_decrypt_key(inputkey, 128, &key);
    
    for (int i = 0 ; i < length ; i += AES_BLOCK_SIZE )
        AES_decrypt(input + i, output + i, &key);
}

/*
  Idential to the above, but encryptes AES in ECB mode
*/
void encryptAES128inECB(unsigned char *input, unsigned char inputkey[], unsigned char *output, int length) {
    AES_KEY key;
    AES_set_encrypt_key(inputkey, 128, &key);

    for (int i = 0 ; i < length ; i += AES_BLOCK_SIZE )
        AES_encrypt(input + i, output + i, &key);
}


/*
  This Function takes as input a name of a file full of hex-encoded cyphertexts and outputs 
  the one that was encrypted using AES in ECB mode.
  Because AES in ECB mode is stateless and deterministic over a 16-byte key, to detect a text
  encrypted with it we could use the edit distance between repeating 16-byte iterations.
 */
string detectAESinECB(string file) {
    ifstream input_file;
    input_file.open(file);
    assert (input_file && "Couldn't open file :(");
  
    double min_distance = 8*16*3;
    string likely_string;
    Hex temp;
    while (getline(input_file, temp.val)) {
        string bin_try = temp.toBin();

        double distance = 0;
        for (int i = 0 ; i < 3 ; i++) {
            distance += editDistance( bin_try.substr(i*8*16, 16*8), bin_try.substr( (i+1)*8*16, 8*16) );
        }
        if (distance < min_distance) {
            min_distance = distance;
            likely_string = temp.val; 
        }
    }
    return likely_string;
}
#endif