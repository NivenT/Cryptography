#ifndef CRYPTOSCHEME_H_INCLUDED
#define CRYPTOSCHEME_H_INCLUDED

#include <BigIntegerLibrary.hh>
#include <vector>
#include <random>

class CryptoScheme {
public:
    CryptoScheme();
    static std::vector<BigUnsigned> encode(const std::string& msg, const BigUnsigned& modulus);
    static std::string decode(const std::vector<BigUnsigned>& enc, const BigUnsigned& modulus);
    static BigUnsigned random(unsigned short numBits);
    static BigUnsigned genPrime(unsigned short numBits);
    static BigUnsigned findGenerator(const BigUnsigned& n);
    static bool isProbablyPrime(const BigUnsigned& num);

    virtual void* encrypt(const BigUnsigned& msg) = 0;
    virtual BigUnsigned decrypt(void* cipher) = 0;
    virtual void init(unsigned short keyLength, const BigUnsigned& modulus) = 0;
};

#endif // CRYPTOSCHEME_H_INCLUDED
