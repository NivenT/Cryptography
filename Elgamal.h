#ifndef ELGAMAL_H_INCLUDED
#define ELGAMAL_H_INCLUDED

#include <utility>

#include "CryptoScheme.h"

typedef std::pair<BigUnsigned, BigUnsigned> Epair;

class Elgamal : public CryptoScheme {
private:
    Epair encrypt(const BigUnsigned& msg, const BigUnsigned& k) const;

    BigUnsigned priv;
    BigUnsigned k;
public:
    Elgamal();
    void init(unsigned short keyLength);
    void* encrypt(const BigUnsigned& msg) const;
    BigUnsigned decrypt(void* cipher) const;
    BigUnsigned getModulus() const;

    BigUnsigned pub;
    BigUnsigned p, g;
    unsigned short keyLength;
};

#endif // ELGAMAL_H_INCLUDED
