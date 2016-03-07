//#define DEBUG

#ifdef DEBUG
    #include <iostream>

    using namespace std;
#endif // DEBUG

#include "Elgamal.h"

Elgamal::Elgamal() {
}

void Elgamal::init(unsigned short keyLength) {
    p = CryptoScheme::genPrime(keyLength+1);
    g = CryptoScheme::genCoprime(p-1);
    priv = CryptoScheme::random(keyLength);
    pub = modexp(g, priv, p);

    #ifdef DEBUG
    cout<<"p:    "<<p<<" ("<<p.bitLength()<<" bits)"<<endl
        <<"g:    "<<g<<" ("<<g.bitLength()<<" bits)"<<endl
        <<"pub:  "<<pub<<" ("<<pub.bitLength()<<" bits)"<<endl
        <<"priv: "<<priv<<" ("<<priv.bitLength()<<" bits)"<<endl
        <<endl;
    #endif // DEBUG
}

Epair Elgamal::encrypt(const BigUnsigned& msg, const BigUnsigned& k) const {
    return std::make_pair(modexp(g, k, p), (msg*modexp(pub, k, p))%p);
}

void* Elgamal::encrypt(const BigUnsigned& msg) const {
    #ifdef DEBUG
        BigUnsigned k = CryptoScheme::random(priv.bitLength());
        Epair cipher = encrypt(msg, k);
        cout<<msg<<" -> ("<<cipher.first<<", "<<cipher.second<<")"<<" (encryption)"<<endl;
        return new Epair(cipher);
    #else
        return new Epair(encrypt(msg, CryptoScheme::random(priv.bitLength())));
    #endif // DEBUG
}

BigUnsigned Elgamal::decrypt(void* cipher) const {
    Epair cipherPair = *(Epair*)cipher;
    #ifdef DEBUG
        BigUnsigned decrypted = (modinv(modexp(cipherPair.first, priv, p), p)*cipherPair.second)%p;
        cout<<"("<<cipherPair.first<<", "<<cipherPair.second<<") -> "<<decrypted<<" (decryption)"<<endl;
        return decrypted;
    #else
        return (modinv(modexp(cipherPair.first, priv, p), p)*cipherPair.second)%p;
    #endif // DEBUG
}

BigUnsigned Elgamal::getModulus() const {
    return p;
}
