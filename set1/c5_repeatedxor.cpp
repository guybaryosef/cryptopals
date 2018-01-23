/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 5 - Repeating-key XOR
 Version 1.0

 Comment: Requires C++11 or later
 By: Guy Bar Yosef
 */

#include <fstream>  //  for input stream
#include <bitset>   // for bitset conversion
#include <cassert>  // for assert function
#include "c2_fixedXOR_1_0.cpp"


string repeatingICExor(string input);   //encrypts a string using repeated-key XOR with key "ICE"

/*
int main() {
    string input;
    cin >>input;
    cout << bintohex(repeatingICExor(input)) << endl;  //printed as hex due to challenge specs
    return 0;
}
*/


/*
 *  Takes a text file as input and returns the contents of that file XORed repeatedly with the key "ICE".
 */
string repeatingICExor(string input) {
    string key = "ICE";             // our specified key

    ifstream input_file(input);     // this constructor requires C++11 or heigher to compile
    assert( input_file.is_open() && "File could not be found :(" );

    string encrypted;       // output
    int count = 0;
    char c;
    input_file >> noskipws >> c;    //reads char by char not skipping whitespace
    while ( !input_file.eof() ) {
        //convert the chars to their respective string of bytes and then XORs them together
        std::bitset<8> bytekey(key[count % 3]);   //cyclcly goes through I,C,E,I,C,...
        std::bitset<8> bytec(c);
        encrypted.append(twoBinXor( bytekey.to_string(), bytec.to_string()  ));
        count++;            //serves to go to the next letter in 'key'
        input_file >> noskipws >> c;
    }
    return encrypted;
}
