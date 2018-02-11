/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 1 - Convert hex to base64
 Version 1.6

 Comment: Will require c++11 or higher

 By: Guy Bar Yosef
 */

#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

using namespace std;

string bintobase64 (string bin);   // converts binary to base64
string hextobin (string hex);      // converts hexadecimal to binary
string bintohex (string bin);      // converts binary to hexadecimal
string base64tobin(string base64); // decodes base64 to binary
string bintoString(string hex1);   // converts hexadecimal to string
string stringtobin(string input);  // converts string to binary

/*
int main() {
    string input;
    cin  >> input;
    cout << bintobase64( hextobin(input)) << endl;
    return 0;
}
*/


/*
 * converts binary digit string to a base64 string object
 */
string bintobase64(string bin) {
    string input = bin;
    string ENCODING64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // base64 encoding schemes

        //makes sure that the base64 characters will be made out of 4 characters, converting each 4 base64 chars to 3 ascii chars.
    int paddingzeros = 0;
    while (input.length() % 24 != 0) {
        input.append(1, '0');
        paddingzeros++;
    }
    int padding = 0;
    if (paddingzeros >= 6)
        padding++;
    if (paddingzeros >= 12)
        padding++;

    string base64;     //final base64 value
    for (int i = 0 ; i < input.length() - paddingzeros; i += 6) {       //iterate through binary digit, at 6-bit value intervals, right to left
        // finding decimal 'value' of current base64 value, which will become index of base64 encoding scheme
        int decvalue = 0;
        for (int j = 5, k = 0; k < 6 ; j--, k++)
            decvalue += (input[i + j] == '0') ? 0 : pow(2, k);
        base64.push_back( ENCODING64[decvalue] );
    }
    base64.append(padding, '=');
    return base64;
}

/*
 * converts hexadecimal string object to a binary string object
 */
string hextobin (string hex) {
    string bin;            //final binary value
    for (int i = 0 ; i < hex.length() ; i++) { //converting hex to binary through switch function
        switch(hex[i]) {
            case '0': bin.append("0000"); break;
            case '1': bin.append("0001"); break;
            case '2': bin.append("0010"); break;
            case '3': bin.append("0011"); break;
            case '4': bin.append("0100"); break;
            case '5': bin.append("0101"); break;
            case '6': bin.append("0110"); break;
            case '7': bin.append("0111"); break;
            case '8': bin.append("1000"); break;
            case '9': bin.append("1001"); break;
            case 'b': bin.append("1011"); break;
            case 'c': bin.append("1100"); break;
            case 'd': bin.append("1101"); break;
            case 'a': bin.append("1010"); break;
            case 'e': bin.append("1110"); break;
            case 'f': bin.append("1111"); break;
        }
    }
    return bin;
}

/*
 *  converts a binary string object to a hexadecimal string object
 */
 string bintohex (string bin) {
    string ENCODING16 = "0123456789abcdef";          // hexadecimal encoding

    int binmod = bin.length() % 4;                   // number of necessary 0s to buffer leftmost hex value
    bin = string( 4 - (binmod), '0' ) + bin;         //adds necessary amount of '0's to beginning to make last hexadecimal digit 4 bits

    string base16;  // output value of
    for (int i = 0 ; i < bin.length() ; i += 4) {    // iterate through every hex value, left to right
        int decvalue = 0;
        for (int j = 3, k = 0 ; j >= 0  ; j--, k++)  // finding decimal value of current hex digit
            decvalue += (bin[i + j] == '0') ? 0 : pow(2, k);
        base16.push_back( ENCODING16[decvalue] );
    }
    return base16;
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
  *  converts binary digit string to ascii equivalent
  */
 string bintoString(string bin) {
     int len = bin.length();     //length of binary string
     bin = (len % 8 == 0 ) ? bin : ( string(8 - (len % 8) , '0' ) + bin);    // add correct amount of 0s to make leftmost byte 8 bits

     string output;
     for (int i = 0 ;  i < len ; i += 8)    // iterates through binary input, byte by byte
         output.push_back( (char) stoi( bin.substr(i, 8) , nullptr , 2 ) );  // converts each byte to equivalent ascii char; requires c++11
     return output;
 }

/*
 * takes in a string of ascii characters and converts it into binary
 */
 string stringtobin(string input) {
     string output;
     for (int i = 0 ; i < input.length() ; i++) {
         bitset <8> temp (input[i]);
         output.append(temp.to_string() );
     }
     return output;
 }
