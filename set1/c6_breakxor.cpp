/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 6 - Break repeating-key XOR
 Version 1.0

 Comment: Requires C++11 or later

 By: Guy Bar Yosef
 */

#include <fstream>
#include <vector>

//includes singleXor, englishScore, bintoString functions as well as #include directive to twoBinXor function
#include "c3_singlexor.cpp"

// Struct to assist with finiding the length of the encryption key (Used in step 1. of the breakXOR function)
typedef struct keyDist {
    double dist;        // edit distance of potitional key length
    int keylen;         // potential key length
} keyDist;

double editDistance(string one, string two); // finds the number of differing bits between two strings
void breakXor(string input_file);            // function to break the Vigenere Cypher
string base64tobin(string base64);           // decodes base64 to binary

int main() {
    string input;
    cin >> input;
    breakXor(input);
    return 0;
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
    for (int i = 0 ; i < 8 ; i++) {
        getline(input_file, begn_buffer);
        begn.append( base64tobin(begn_buffer) );
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

        while ( getline(input_file, buffer_helper) )     // reads in entire input file into 'buffer' string.
                buffer.append( base64tobin(buffer_helper) );

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
        for (int k = 0 ; k < keyBlocks.size() ; k++ )
            possiblemessage.append(bintoString(twoBinXor(keyBlocks[k], binPossibleKey)));

        // Step 4.)
        if (englishScore(possiblemessage) > messagescore) {
            messagescore = englishScore(possiblemessage);
            message = possiblemessage;
            key = possibleKey;
        }
    }
    // The writing of the correct key and message to output file 'c6_output.txt'.
    ofstream output_file("c6_output.txt");
    output_file << "Key: " << key << '\n' << "Message: " << endl;
    output_file << message << endl;
}


/*
 Converts a string in base64 to binary with the encoding scheme defined in challenge 1.
    - Note: This function assums that the base64 input is correct, as in an actual character length encoded to base64. In other words this function assumes that: [ base64.length() % 4 = 0].
 */
string base64tobin(string input) {
    string output;
    for (int i = 0 ; i < input.length() ; i++) {
        int cur;
        if (input[i] >= 'a' && input[i] <= 'z')
            cur = input[i] - 'a' + 26;
        else if (input[i] >= 'A' && input[i] <= 'Z')
            cur = input[i] - 'A';
        else if (input[i] >= '0' && input[i] <= '9')
            cur = input[i] - '0' + 52;
        else if (input[i] == '+')
            cur = 62;
        else if (input[i] == '/')
            cur = 63;
        else        // as this function assumes a correct input, this will only occur if the current char is '='.
            cur = 0;
        bitset <6> temp (cur);
        output.append( temp.to_string() );
    }
    return output;
}


/*
  This funciton computes the Hamming Distance between two strings.
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
