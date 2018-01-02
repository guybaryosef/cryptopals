/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 2 - Fixed XOR
 Version 1.0

 By: Guy Bar Yosef
 */

#include "c1_HextoBase64.cpp"

string twoBinXor(string hex1, string hex2);

int main() {

    string hex1, hex2;
    cin >> hex1 >> hex2;

    cout << twoBinXor(hex1, hex2) << endl;

    return 0;

}


/*
 *  takes two equal-length buffers and outputs their XOR combination
 */
string twoBinXor( string hex1, string hex2) {
    string bin1 = hextobin(hex1);           //converts from hexadecimal to binary
    string bin2 = hextobin(hex2);

    string output = "";
    int len = bin1.length();

    for (int i = 0 ; i < len ; i++) {       //XORs each bit in the two buffers
        if ( bin1[i] != bin2[i] )
            output.append("1");
        else
            output.append("0");
    }

    return bintohex(output);               // returns the hexadecimal value of the binary XOR combo
}
