#ifndef CRYPTOSCHEME_H_INCLUDED
#define CRYPTOSCHEME_H_INCLUDED

#include <BigIntegerLibrary.hh>
#include <vector>
#include <random>

class CryptoScheme {
private:
    static std::mt19937_64 rng;
public:
    CryptoScheme();
    static std::vector<BigUnsigned> encode(const std::string& msg);
    static std::string decode(const std::vector<BigUnsigned>& enc);
    static BigUnsigned random(unsigned short numBits);
    static BigUnsigned genPrime(unsigned short numBits);
    static BigUnsigned genCoprime(const BigUnsigned& n);
    static BigUnsigned findOrder(const BigUnsigned& n, const BigUnsigned& modulus);
    static BigUnsigned primePi(const BigUnsigned& n); ///used to test accuracy of isProbablyPrime
    static bool isProbablyPrime(const BigUnsigned& num);
    static void seedRNG();

    virtual BigUnsigned getModulus() const = 0;
    virtual BigUnsigned decrypt(void* cipher) const = 0;
    virtual void* encrypt(const BigUnsigned& msg) const = 0;
    virtual void init(unsigned short keyLength) = 0;
};

#endif // CRYPTOSCHEME_H_INCLUDED
