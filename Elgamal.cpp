#include "Elgamal.h"
/*
#include <iostream>

using namespace std;
*/
Elgamal::Elgamal() {
}

void Elgamal::init(unsigned short keyLength, const BigUnsigned& modulus) {
    this->keyLength = keyLength;
    p = modulus;
    priv = CryptoScheme::random(keyLength);
    g = CryptoScheme::findGenerator(priv);
    pub = modexp(g, priv, p);
    /*
    cout<<"p: "<<p<<endl
        <<"g: "<<g<<endl
        <<"pub: "<<pub<<endl
        <<"priv: "<<priv<<endl;
    */
}

Epair Elgamal::encrypt(const BigUnsigned& msg, const BigUnsigned& k) {
    return std::make_pair(modexp(g, k, p), (msg*modexp(pub, k, p))%p);
}

void* Elgamal::encrypt(const BigUnsigned& msg) {
    return new Epair(encrypt(msg, CryptoScheme::random(keyLength)));
}

BigUnsigned Elgamal::decrypt(void* cipher) {
    Epair cipherPair = *(Epair*)cipher;
    return (modinv(modexp(cipherPair.first, priv, p), p)*cipherPair.second)%p;
}
