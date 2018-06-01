/*
 Cryptopals Crypto Challenges
 Set 2
 Functions used for Set 2

 By: Guy Bar Yosef
 */

#include <string>
#include <iostream>

using namespace std;

string implementPKSC7(string & input, int blocksize); // ipmlements PKSC#7 padding


/*
 *  Implements PKSC#7 padding for an inputted string, based on the inputed blocksize.
 *  The padding works by finding the amount of bytes needed to make the input a multiple of the blocksize,
 *  and then adding that value that amount of times to the end of the string.
 *      EX (in hex): 03 03 03
 *                   04 04 04 04 
 */
string implementPKSC7( string & input, int blocksize ) {
    int paddingSize = blocksize - (input.length() % blocksize);
    if (paddingSize == blocksize)
        paddingSize = 0;
    
    // need to create the correct padding
    int size = to_string(blocksize).length();
    string pad = to_string(paddingSize);
    pad.insert(0, string(size-pad.length(), '0') );

    string padding;
    for (int i = 0 ; i < paddingSize ; ++i)
        padding.append(pad);

    string output = input + padding;
    return output;
}