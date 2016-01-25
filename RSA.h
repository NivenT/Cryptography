#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include "CryptoScheme.h"

class RSA : public CryptoScheme {
private:
    BigUnsigned priv;
public:
    RSA();
    void init(unsigned short keyLength);
    void* encrypt(const BigUnsigned& msg) const;
    BigUnsigned decrypt(void* cipher) const;
    BigUnsigned getModulus() const;

    BigUnsigned pub;
    BigUnsigned n;
    unsigned short keyLength;
};

#endif // RSA_H_INCLUDED
