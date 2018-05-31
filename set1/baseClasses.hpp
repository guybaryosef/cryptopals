/*
 Cryptopals Crypto Challenges
 Set 1
 Base Structs headerfiles

 By: Guy Bar Yosef
*/

#ifndef BASES
#define BASES

#include <string>

struct Bin {
    std::string val;
    Bin(std::string = std::string());

    std::string fromString();
    std::string toString();
    std::string toHex();
    std::string toBase64();
};

struct Base64 {
    std::string val;
    Base64(std::string = std::string());

    std::string toHex();
    std::string toBin();
};

struct Hex {
    std::string val;
    Hex(std::string = std::string());

    std::string toBase64();
    std::string toBin();
};

#endif