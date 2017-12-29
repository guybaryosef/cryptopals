/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 1 - Convert hex to base64
 Version 1.0

 By: Guy Bar Yosef
 */


#include <string>
#include <cmath>
#include <iostream>

using namespace std;

string hex2bin (char hex);  //function converting hexadecimal values to binary
string ENCODING64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // base64 encoding scheme

int main () {

    string hex;             // inputted hex value
    string bin = "";        //intermediate binary value
    string base64 = "";     //output base64 value
    cin >> hex;

    int hexlen = hex.length();
    int binlen = hexlen*4;                          //total binary value length

    for (int i = 0 ; i <= hexlen ; i++)             //converting hex to binary through hex2bin function
        bin.append( hex2bin(hex[i]) );

    for (int i = binlen - 1 ; i >= 0 ; i -= 6) {    //iterate through 6-bit values, right to left
        string current;                             // the 6 bit base64 value
        if (i < 5) {                                // at final iteration, final base64 value could be < 6 bits
            int bufzeroscount = 5 - i;              // amount of 0s to add to make 'current' a 6-bit value
            string bufzero = "0";
            for (int j = bufzeroscount ; j > 1 ; j--)
                bufzero.append("0");
            current = bufzero + bin.substr(0, i + 1); //combining
        }
        else                                        // current will be a 6-bit value
            current = bin.substr(i - 5, 6);

        int value = 0;
        for (int j = 5, k = 0 ; j >= 0  ; j--, k++) // finding decimal 'value' of current,--> will become index of base64 encoding scheme
            value += (current[j] == '0') ? 0 : pow(2, k);
        base64 = ENCODING64[value] + base64;
    }

    cout << base64 << endl;
    return 0;
}

/*
 * Uses a switch statement to convert a hexadecimal value to its 4 bit binary equivalent
 */
string hex2bin (char hex) {
    switch(hex) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'a': return "1010";
        case 'b': return "1011";
        case 'c': return "1100";
        case 'd': return "1101";
        case 'e': return "1110";
        case 'f': return "1111";
        }
}
