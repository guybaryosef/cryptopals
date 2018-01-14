/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 1 - Convert hex to base64
 Version 1.3

 By: Guy Bar Yosef
 */

#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

string bintobase64 (string bin);  // converts binary to base64
string hextobin (string hex);     // converts hexadecimal to binary
string bintohex (string bin);     // converts binary to hexadecimal


/*
 * converts binary string object to a base64 string object
 */
string bintobase64(string bin) {

    string ENCODING64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // base64 encoding scheme

    int len = bin.length();
    int binmod = len % 6;
    bin = (binmod == 0 ) ? bin : ( string(6 - binmod, '0') + bin ) ;

    string base64 = "";     //final base64 value
    for (int i = 0 ; i < len ; i += 6) {       //iterate through binary digit, at 6-bit value intervals, right to left
        int decvalue = 0;
        for (int j = 5, k = 0 ; j >= 0  ; j--, k++)    // finding decimal 'value' of current,--> will become index of base64 encoding scheme
            decvalue += (bin[i + j] == '0') ? 0 : pow(2, k);

        base64.push_back( ENCODING64[decvalue] );
    }
    return base64;
}

/*
 * converts hexadecimal string object to a binary string object
 */
string hextobin (string hex) {

    string bin = "";            //final binary value
    int hexlen = hex.length();

    for (int i = 0 ; i < hexlen ; i++)     //converting hex to binary through switch function
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
    return bin;
}

/*
 *  converts a binary string object to a hexadecimal string object
 */
 string bintohex (string bin) {

    int binmod = bin.length() % 4;
    bin = (binmod == 0) ? bin : ( string( 4 - (binmod), '0' ) + bin );   //adds necessary amount of '0's to beginning to make last hexadecimal digit 4 bits

    long long decvalue = 0;            // long long as to let input binary value be large
    for (int i = bin.length() - 1, k = 0 ; i >= 0 ; i--, k++ )  //iterate right to left, raising each bit by 2^(its posisition); i.e. binary -> decimal
        decvalue += (bin[i] == '0') ? 0 : pow(2, k);

    stringstream ss;
    ss << hex << decvalue;  // converts decimal to hexadecimal through stream
    return ss.str();        // returns a copy of stream ss as a string
 }
