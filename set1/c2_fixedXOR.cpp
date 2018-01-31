/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 2 - Fixed XOR
 Version 1.1

 By: Guy Bar Yosef
 */

#include "c1_HextoBase64.cpp"

string twoBinXor(string bin1, string bin2);  //XORs two binary strings

/*
int main() {
    string hex1, hex2;
    cin >> hex1 >> hex2;
    string bin1 = hextobin(hex1);
    string bin2 = hextobin(hex2);
    cout << twoBinXor(hex1, hex2) << endl;
    return 0;
}
*/

/*
 * takes two equal-length buffers and outputs their XOR combination
 */
string twoBinXor(string bin1, string bin2) {
    if (bin1.length() != bin2.length()) {   // buffers the shorter input with '0's
        int difference = abs((int)bin1.length() - (int)bin2.length() );
        if (bin1.length() > bin2.length())
            bin1 = bin1.substr(0, bin1.length() - difference);
        else
            bin2 = bin2.substr(0, bin2.length() - difference);
    }

    string output;
    for (int i = 0 ; i < bin1.length() ; i++) {   //XORs each bit
        if ( bin1[i] != bin2[i] )
            output.append("1");
        else
            output.append("0");
    }
    return output;      // returns the binary value of the binary XOR combo
}
