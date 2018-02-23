/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 8 - Detect AES in ECB mode
 Version 1.0

 COMMENTS:
   - Requires C++11 or higher to compile

 By: Guy Bar Yosef
 */

// Includes the hamming distance function as well as several other basica functions.
#include "c6_breakxor.cpp"

string detectAESinECB(string file);    //detects the AES in ECB mode encryption of a file.


int main() {
    string input, ciphertext;
    cin >> input;
    try {
        ciphertext = detectAESinECB(input);
    }
    catch (string error) {
        cout << error << endl;
    }
    cout << ciphertext << endl;
    return 0;
}


/*
 *  This Function takes as input a name of a file full of hex-encoded cyphertexts and outputs 
 *  the one that was encrypted using AES in ECB mode.
 *  Because AES in ECB mode is statless and deterministic over a 16-byte key, to detect a text
 *  encrypted with it we could use the Hamming Distance between repeating 16-byte iterations.
 */
string detectAESinECB(string file) {
    ifstream input_file;
    input_file.open(file);
    assert (input_file && "Couldn't open file :(");

        
    double min_distance = 8*16;
    string likely_string;
    string temp;
    while (getline(input_file, temp)) {
        string bin_try = hextobin(temp);
        double distance = 0;
        for (int i = 0 ; i < 3 ; i++) {
            distance += editDistance( bin_try.substr(i*8*16, 16*8), bin_try.substr( (i+1)*8*16, 8*16) );
        }
        if (distance / 3 < min_distance) {
            min_distance = distance/3;
            likely_string = temp; 
        }
    }
    return likely_string;
}
