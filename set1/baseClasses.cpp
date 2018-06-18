/*
 Cryptopals Crypto Challenges
 Set 1
 Base Structs definitions

 Comment: requires c++11 or higher

 By: Guy Bar Yosef
 */

#include <string>
#include <cmath>
#include <iomanip>
#include <bitset>

#include "baseClasses.hpp"

using namespace std;


/*
 Binary class definitions
*/

// constructor
Bin::Bin(string input, string type) {
    if (type == "bin")
        val = input;
    else
        val = fromString(input); 
    }

// takes in a string of ascii characters and converts it into binary
string Bin::fromString(string input) {
    string output;
    for (int i = 0 ; i < input.length() ; i++) {
        bitset <8> temp (input[i]);
        output.append(temp.to_string() );
    }
    return output;
}

// converts binary digit string to ascii equivalent
string Bin::toString() {
    string temp = val;
    int len = temp.length();
    temp = (len % 8 == 0 ) ? temp : ( string(8 - (len % 8) , '0' ) + temp);    // add correct amount of 0s to make leftmost byte 8 bits

    string output;
    for (int i = 0 ;  i < len ; i += 8)    // iterates through binary input, byte by byte
        output.push_back( (char) stoi( temp.substr(i, 8) , nullptr , 2 ) );  // converts each byte to equivalent ascii char; requires c++11
    return output;
}

// converts a binary string object to a hexadecimal string object
string Bin::toHex () {
    string temp = val;
    string ENCODING16 = "0123456789abcdef";    // hexadecimal encoding

    //adds necessary amount of '0's to beginning to make last hexadecimal digit 4 bits           
    temp = (temp.length()%4 == 0) ? temp : string( 4 - (temp.length()%4), '0' ) + temp;

    string hex(temp.length() / 4,0);  //our output
    auto it = hex.begin();
    for (int i = 0 ; i < temp.length() ; i += 4, ++it) {    // iterate through every hex value, left to right
        int decvalue = 0;
        for (int j = 3, k = 0 ; j >= 0  ; j--, k++)  // finding decimal value of current hex digit
            decvalue += (temp[i + j] == '0') ? 0 : pow(2, k);
        *it = ENCODING16[decvalue];
    }
    return hex;
}

// converts binary digit string to a base64 string object
string Bin::toBase64() {
    string temp = val;
    string ENCODING64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // base64 encoding schemes

    //makes sure that the base64 characters will be made out of 4 characters, converting each 4 base64 chars to 3 ascii chars.
    int padding = 0;
    int zeroCount = 24 - (temp.length() % 24);
    if (zeroCount == 24) {
        zeroCount = 0;
    }
    else {
        temp.append(string(zeroCount, '0') );

        if (zeroCount >= 6)
            padding++;
        if (zeroCount >= 12)
            padding++;
    }

    string output = string(temp.length()/6, 0);
    auto it = output.begin();
    for (int i = 0 ; i < temp.length() - zeroCount ; i += 6, ++it) {  //iterate through binary temp, at 6-bit intervals
        // finding decimal value of current base64 value, which will become index of base64 encoding scheme
        int decvalue = 0;
        for (int j = 5, k = 0; k < 6 ; j--, k++)
            decvalue += (temp[i + j] == '0') ? 0 : pow(2, k);
        *it = ENCODING64[decvalue];
    }
    output.append(padding, '=');
    return output;
}



/*
 Base64 class definitions
*/

// constructor
Base64::Base64(string input) { val = input; }

// convert a base64 string to a hexadecimal string
string Base64::toHex() {
    Bin temp (toBin());
    return temp.toHex();
}

/*
    Converts a string in base64 to binary with the encoding scheme defined in challenge 1.
    - Note: This function assums that the base64 input is correct, as in an actual character length encoded to base64. In other words this function assumes that: [ base64.length() % 4 = 0].
*/
string Base64::toBin() {
    string output;
    for (int i = 0 ; i < val.length() ; i++) {
        int cur;
        if (val[i] >= 'a' && val[i] <= 'z')
            cur = val[i] - 'a' + 26;
        else if (val[i] >= 'A' && val[i] <= 'Z')
            cur = val[i] - 'A';
        else if (val[i] >= '0' && val[i] <= '9')
            cur = val[i] - '0' + 52;
        else if (val[i] == '+')
            cur = 62;
        else if (val[i] == '/')
            cur = 63;
        else  // as this function assumes a correct input, this will only occur if the current char is '='.
            cur = 0;
        bitset <6> temp (cur);
        output.append( temp.to_string() );
    }
    return output;
}


/*
 Hexadecimal class definition
*/

// constructor
Hex::Hex(string input) { val = input; }

// Converts a hexadecimal string to a base64 string
string Hex::toBase64 () {
    string buf = toBin();
    Bin temp (toBin());
    return temp.toBase64();
}

// converts hexadecimal string object to a binary string object
string Hex::toBin () {
    string bin;            //final binary value
    for (int i = 0 ; i < val.length() ; i++) { //converting hex to binary through switch function
        switch(val[i]) {
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



