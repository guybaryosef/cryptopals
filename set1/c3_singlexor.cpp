/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 3 - Single-byte XOR cipher
 Version 1.0

 By: Guy Bar Yosef
 */

#include <map>                         // for map used in scoring function
#include <algorithm>                   // for converting message to lowercase
#include <bitset>                      // for converting char to string of binary
#include <cassert>                      // for the assert function

#include "c2_fixedXOR_1_0.cpp"         // for twoBinXor function

string singleXor(string bininput);     // takes a binary input and returns the encrypted string
double englishScore(string input);     // scores text, higher score relates to higher chance to be english
string bintoString(string hex1);       // converts hexadecimal to string


int main () {
    string hexinput;                        // hexadecimal input
    cin >> hexinput;
    string bininput = hextobin(hexinput);   // binary value of input
    cout << "Message: " << singleXor(bininput) << endl;
    return 0;
}

/*
 * Takes binary input and returns a string that was encrypted through an XOR with a single ascii char
 * Main idea here is that: [ message XOR key XOR key = message ], and our input was [message XOR key], the encrypted message
 */
 string singleXor(string bininput) {

     string correctmessage;        // the decrypted message
     double maxscore = 0;          // the maxscore to keep track of through iterations below

     //iterate through all ascii values, finding the one which produces the heighest english score, signifying how close the potential message is in english
     for (unsigned char c = 0 ; c < 128 ; c++) {
         std::bitset<8> bytekey(c);            //convert each ascii char to its 8 bit binary value, to use in XOR with encrypted message
         string binkey = bytekey.to_string();

         string possiblekey = binkey;         //'possiblekey' will be a string the length of the decrypted message, populated with one repeating char

         // possiblekey needs to be the same length as binary input, as the func twoBinXor assumes it to be
         while (possiblekey.length() < bininput.length() )
             possiblekey += binkey;
         assert(possiblekey.length() == bininput.length() && "The encrypted message is an incorrect amount of bits for an ascii message!");

         string message = bintoString( twoBinXor(possiblekey, bininput) );
         double score = englishScore(message);

         if (score > maxscore) {  // if we have a new high-english-score message, make that the most likely candidate
             maxscore = score;
             correctmessage = message;
         }
     }
     return correctmessage;
 }

/*
 *  scores text, higher score relates to higher chance to be english
 */
double englishScore(string input) {
    // letter weights derived from https://upload.wikimedia.org/wikipedia/commons/d/d5/English_letter_frequency_%28alphabetic%29.svg
    map < char , double  > scorecard;
        scorecard['a'] = 0.082;     scorecard['b'] = 0.016;
        scorecard['c'] = 0.028;     scorecard['d'] = 0.043;     scorecard['e'] = 0.128;
        scorecard['f'] = 0.022;     scorecard['g'] = 0.020;     scorecard['h'] = 0.061;
        scorecard['i'] = 0.071;     scorecard['j'] = 0.003;     scorecard['k'] = 0.006;
        scorecard['l'] = 0.039;     scorecard['m'] = 0.023;     scorecard['n'] = 0.070;
        scorecard['o'] = 0.076;     scorecard['p'] = 0.019;     scorecard['q'] = 0.002;
        scorecard['r'] = 0.061;     scorecard['s'] = 0.062;     scorecard['t'] = 0.091;
        scorecard['u'] = 0.028;     scorecard['v'] = 0.009;     scorecard['w'] = 0.023;
        scorecard['x'] = 0.003;     scorecard['y'] = 0.019;     scorecard['z'] = 0.001;
        scorecard['0'] = 0.010;     //assign to numbers equivalent score, except 0, which is slightly more popular (arbitrarily decided)
        scorecard['1'] = 0.008;     scorecard['2'] = 0.008;     scorecard['3'] = 0.008;
        scorecard['4'] = 0.008;     scorecard['5'] = 0.008;     scorecard['6'] = 0.008;
        scorecard['7'] = 0.008;     scorecard['8'] = 0.008;     scorecard['9'] = 0.008;
        scorecard[' '] = 0.068;

    double score = 0;
    transform( input.begin(), input.end(), input.begin(), ::tolower );          // converts input to lowercase, so as to be an index in scorecard map
    for (string::iterator it = input.begin() ; it < input.end() ; it++ ) {      // iterate through string, summing up its letter scores
        score += scorecard[ *it ];
    }
    return score;
}

/*
 *  converts hexadecimal value to string equivalent
 */
string bintoString(string bin) {
    int len = bin.length();     //length of binary string
    bin = (len % 8 == 0 ) ? bin : ( string(8 - (len % 8) , '0' ) + bin);    // add correct amount of 0s to make leftmost byte 8 bits

    string output = "";
    for (int i = 0 ;  i < len ; i += 8)                                     // iterates through binary input, byte by byte
        output.push_back( (char) stoi( bin.substr(i, 8) , nullptr , 2 ) );  // converts each byte to equivalent ascii char; requires c++11

    return output;
}
