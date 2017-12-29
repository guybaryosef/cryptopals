/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 1 - Convert hex to base64
 Version 1.1

 By: Guy Bar Yosef
 */

#include <string>
#include <cmath>
#include <iostream>

using namespace std;

string hex2bin (string hex);     // converts hexadecimal values to binary
string bin2base64 (string bin);  // converts binary to base64

int main () {

    string hex;             // inputted hex value
    cin >> hex;

    string bin = hex2bin(hex);          //intermediate binary value
    string base64 = bin2base64(bin);    //output base64 value

    cout << base64 << endl;
    return 0;
}

/*
 * converts binary digit to base64
 */
string bin2base64(string bin) {

    string base64 = "";     //final base64 value
    int binlen = bin.length();
    string ENCODING64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // base64 encoding scheme

    for (int i = binlen - 1 ; i >= 0 ; i -= 6) {    //iterate through binary digit, at 6-bit value intervals, right to left
        string current;                             // the 6-bit value in decimal
        if (i < 5) {                                // dealing w/ final iteration, final base64 value could be < 6 bits
            int bufzeroscount = 5 - i;              // amount of 0s to add to make 'current' a 6-bit value
            string bufzero = "0";
            for (int j = bufzeroscount ; j > 1 ; j--)
                bufzero.append("0");
            current = bufzero + bin.substr(0, i + 1); // the final 6-bit digit, w/ zero buffer
        }
        else                                        // all other iteration except maybe final iteration
            current = bin.substr(i - 5, 6);

        int value = 0;
        for (int j = 5, k = 0 ; j >= 0  ; j--, k++) // finding decimal 'value' of current,--> will become index of base64 encoding scheme
            value += (current[j] == '0') ? 0 : pow(2, k);
        base64 = ENCODING64[value] + base64;
    }
    return base64;
}

/*
 * converts hexadecimal digit to binary
 */
string hex2bin (string hex) {

    string bin = "";            //final binary value
    int hexlen = hex.length();

    for (int i = 0 ; i < hexlen ; i++)     //converting hex to binary through 
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
