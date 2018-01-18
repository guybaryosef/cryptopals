/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 4 - Detect single-character XOR
 Version 1.0

 Comment: requires c++11 or heigher to compile
 
 By: Guy Bar Yosef
 */

#include <fstream>                           // used for the I/O file

#include "c3_singlexor.cpp"                  // used for the singleXor function - requires c++11 to run due to stoi func.

message_score detectxor(string filename);    // finds the single-xor encrypted message from a file full of encrypted messages

/*
int main() {
    string inputname;
    cout << "Please enter the input file path & name: ";
    cin >> inputname;
    message_score decrypted = detectxor(inputname);
    cout << "The message is: " << decrypted.message;
    cout << "The key is: " << decrypted.key << endl;
    cout << "The hex encoded string is: " << decrypted.hexencoded << endl;
    return 0;
}
*/

/*
 Takes in a file name and path which contains any amount of hex encoded strings among which one was encrypted with a single-character XOR.
 This function finds the encrypted message by comparing the heighest-english-scored message of each hex encoded string and returning the heightest score of them all.
 */
message_score detectxor(string filename) {

    ifstream input_file (filename);
    assert( input_file.is_open() && "File could not be found :(" );    // stops program if file doesn't or couldn't be found

    message_score decrypted;         // the output of the function, the decrypted message struct

    string hexcandidate;
    input_file >> hexcandidate;
    while( !input_file.eof() ) {     // iterates through the hex-encoded strings inside the input file
        string bincandidate = hextobin(hexcandidate);
        message_score candidate = singleXor(bincandidate);  // returns the heighest-english-scored key and message of the hex encoded candidate
        if (candidate.score > decrypted.score)              // compares with the heighest scored message up until now in file
            decrypted = candidate;
        input_file >> hexcandidate;
    }
    return decrypted;
}
