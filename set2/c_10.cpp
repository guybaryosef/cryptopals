/*
 Cryptopals Crypto Challenges
 Set 2
 Challenge 10 - Implement CDC mode

 By: Guy Bar Yosef
 */

#include "functions2.cpp"

int main() {
    //testing the decrypt CBC function using the provided input file 'c_10input.txt' and outputing in 'c_10output.txt'.
    unsigned char inputKey[] = "YELLOW SUBMARINE";
    string inputf = "c_10input.txt";
    char IV[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

    ifstream input_file(inputf);
    assert(input_file && "unable to open file :{");

    Bin bininput;
    Base64 buf1;
    while (getline(input_file, buf1.val))
        bininput.val.append( buf1.toBin() );
    
    string temp = bininput.toString();

    unsigned char input[temp.length()+1]; // containes the encrypted string from the input file
    for (int i = 0 ; i < temp.length() ; ++i)
        input[i] = temp[i];
    input[temp.length()] = '\0';

    unsigned char output[temp.length()+1];    // will contain the decyphered text
    output[temp.length()] = '\0';
    
    decryptAES128inCBC(IV, input, inputKey, output, temp.length() );

    ofstream output_file("c_10output.txt");
    output_file << output << endl;

    input_file.close();
    output_file.close();
    
    ////////////////////////////////////////
    // encrypting and decrypting my own file, 'c_10input2.txt', and outputing the result as 'c_10output2.txt'.
    ////////////////////////////////////////
    
    input_file.open("c_10input2.txt");
    assert(input_file && "unable to open file :{");

    output_file.open("c_10output2.txt");
    assert(output_file && "Cannot create/overwrite output file :{");

    string buffer;
    string totalinput;
    while (getline(input_file, buffer))
        totalinput.append( buffer+ "\n");

    output_file << "Original Message: " << endl << totalinput << endl;

    totalinput = implementPKSC7(totalinput, AES_BLOCK_SIZE); // pad the plaintext

    unsigned char input2[totalinput.length()+1];   // the input of the plaintext file
    for (int i = 0 ; i < totalinput.length() ; ++i)
        input2[i] = totalinput[i];
    input2[totalinput.length()] = '\0';

    unsigned char encryptedText[totalinput.length() +1];    // the outputted encrypted text
    encryptedText[totalinput.length()] = '\0';

    encryptAES128inCBC(IV, input2, inputKey, encryptedText, totalinput.length());

    output_file << "\n\n" << "Encrypted Message: " << endl << encryptedText << endl;

    unsigned char decryptedText[totalinput.length()+1];
    decryptedText[totalinput.length()] = '\0';

    decryptAES128inCBC(IV, encryptedText, inputKey, decryptedText, totalinput.length() );

    output_file << "\n\n" << "Decrypted message: " << endl << decryptedText << endl;

    return 0;
}
