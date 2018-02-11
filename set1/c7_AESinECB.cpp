/*
 Cryptopals Crypto Challenges
 Set 1
 Challenge 7 - AES in ECB mode
 Version 1.0

 COMMENTS:
    - Requires C++11 or higher to compile
    - Need to link -lcrypto to compile
        -overall to compile: "clang++ -std=c++11 -lcrypto c7_AESinECB.cpp"

 By: Guy Bar Yosef
 */

#include <openssl/evp.h>
#include <fstream>
#include "c1_HextoBase64.cpp"

#define KEY_SIZE 16  //16 *sizeof(unsigned char) = 16 * 8 = 128 = mode of AES we are using

void decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext);  //decrypts a string in AES 128 in ECB mode
void decryptAES128inECB(string inputfile, string inputkey);         //Overall function that prepares the input for and then runs the funtion 'decrypt'.

int main(void) {
    string input;
    cout << "Please enter file name to read from: ";
    cin >> input;
    string key = "YELLOW SUBMARINE";        // given in this challenge
    try {
        decryptAES128inECB(input, key);
    }
    catch(string error) {
        cerr << error << endl;
        return -1;
    }
    return 0;
}

/*
 * Takes as input a base64- encoded file that has been encrypted via AES-128 in ECB mode under the specified key.
    Outputs the decrypted message to c7_output.txt
 */
void decryptAES128inECB(string inputfile, string inputkey) {
    ifstream input_file(inputfile);
    if (!input_file)
        throw "File not found :(.";

    string bininput, temp;
    while (getline(input_file, temp))
        bininput.append( base64tobin(temp) );
    string input = bintoString(bininput);

    // the following are types as specified by the decrypt function below
    unsigned char *key = (unsigned char *) inputkey.c_str();       // AES cipher key
    unsigned char decryptedtext[input.length()];                   // Buffer for the decrypted text
    unsigned char *ciphertext = (unsigned char *)input.c_str();    // the encrypted test, in ascii

    decrypt(ciphertext, input.length(), key, decryptedtext);

    string output(reinterpret_cast<char*> (decryptedtext));

    // checks for padding and removes it if found.
    for (int i = 15 ; i > 0 ; i--)
        if ((unsigned char)(output[output.length() - i]) == 4 ||   // the 4 ascii char is the End-Of-Transimission char
            (unsigned char)(output[output.length() - i]) > 126)    // after 126 ascii becomes non- english chars
            output = output.substr(0, output.length() - i );

    ofstream output_file("c7_output.txt");
    output_file << output << endl;
}

/*
 *   Very heavily derived from the OpenSSLWiki  page: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 */
void decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext)
{
    /* Create and initialise the context */
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // Initialise the decryption operation.
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);

    // Provide the message to be decrypted, and obtain the plaintext output.
    // EVP_DecryptUpdate can be called multiple times if necessary.
    int len;
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    int plaintext_len = len;

    //Finalise the decryption. Further plaintext bytes may be written at this stage.
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    return;
}
