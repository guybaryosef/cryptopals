/*
 Cryptopals Crypto Challenges
 Set 2
 Challenge 9 - Implement PKCS#7 padding
 Version 1.0

<<<<<<< HEAD

=======
>>>>>>> b02ba5178791491e6c13b3281e9fc36552601bbd
 By: Guy Bar Yosef
 */

#include <string>
#include <iostream>

using namespace std;


void implementPKSC7(string & input, int blocksize); // ipmlements PKSC#7 padding

/*
int main () {
    string input;
    int block;
    cin >> input >> block;
    implementPKSC7(input, block);
    return 0;
}
*/


/*
 *  Implements PKSC#7 padding for an inputted string, based on the inputed blocksize.
 *  The padding works by finding the amount of bytes needed to make the input a multiple of the blocksize,
 *  and then adding that value that amount of times to the end of the string.
 *      EX (in hex): 03 03 03
 *                   04 04 04 04 
 */
void implementPKSC7( string & input, int blocksize ) {
    int padding_size = input.length() % blocksize;
    string to_append = std::string((char)padding_size , padding_size);
    input = input + to_append;
<<<<<<< HEAD
}
=======
}
>>>>>>> b02ba5178791491e6c13b3281e9fc36552601bbd
