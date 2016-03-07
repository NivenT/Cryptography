#ifndef ECELGAMAL_H_INCLUDED
#define ECELGAMAL_H_INCLUDED

#include "CryptoScheme.h"
#include "EllipticCurve.h"

typedef std::pair<ECPoint, FFElement> ECEpair;

///Menezes-Vanstone variant of Elliptic Elgamal
class ECElgamal : public CryptoScheme {
private:
    ECEpair encrypt(const BigUnsigned& msg, const BigUnsigned& k) const;

    EllipticCurve curve;
    BigUnsigned priv;
public:
    ECElgamal();
    BigUnsigned getModulus() const;
    BigUnsigned decrypt(void* cipher) const;
    void* encrypt(const BigUnsigned& msg) const;
    void init(unsigned short keyLength);

    BigUnsigned p;
    ECPoint pubP, pubQ;
};

#endif // ECELGAMAL_H_INCLUDED
