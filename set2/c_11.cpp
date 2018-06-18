/*
 Cryptopals Crypto Challenges
 Set 2
 Challenge 11 - An ECB/CBC detection oracle

 By: Guy Bar Yosef
 */

#include "functions2.cpp"


int main() {
    ifstream input_file("c_11input.txt"); 

    string input;
    string buffer;
    while (getline(input_file, buffer))
        input.append(buffer + "\n");

    string actualMode;
    int size;
    unsigned char *output = encryptionOracle(input, actualMode, size);

    cout << "size: " << size << endl;

    string posMode = decryptionOracle(output, size);
    cout << "Actual mode: " << actualMode << endl << "Detected mode: " << posMode << endl;

    free(output);
    return 0;
}