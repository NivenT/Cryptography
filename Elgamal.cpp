//#define DEBUG

#ifdef DEBUG
    #include <iostream>

    using namespace std;
#endif // DEBUG

#include "Elgamal.h"

Elgamal::Elgamal() {
}

void Elgamal::init(const BigUnsigned& modulus) {
    this->keyLength = modulus.bitLength()-1;
    p = modulus;
    g = CryptoScheme::findGenerator(p-1);
    priv = CryptoScheme::random(keyLength);
    pub = modexp(g, priv, p);

    #ifdef DEBUG
    cout<<"p:    "<<p<<endl
        <<"g:    "<<g<<endl
        <<"pub:  "<<pub<<endl
        <<"priv: "<<priv<<endl
        <<endl;
    #endif // DEBUG
}

Epair Elgamal::encrypt(const BigUnsigned& msg, const BigUnsigned& k) {
    return std::make_pair(modexp(g, k, p), (msg*modexp(pub, k, p))%p);
}

void* Elgamal::encrypt(const BigUnsigned& msg) {
    #ifdef DEBUG
        BigUnsigned k = CryptoScheme::random(keyLength);
        Epair cipher = encrypt(msg, k);
        cout<<msg<<" -> ("<<cipher.first<<", "<<cipher.second<<")"<<endl;
        return new Epair(cipher);
    #else
        return new Epair(encrypt(msg, CryptoScheme::random(keyLength)));
    #endif // DEBUG
}

BigUnsigned Elgamal::decrypt(void* cipher) {
    Epair cipherPair = *(Epair*)cipher;
    #ifdef DEBUG
        BigUnsigned decrypted = (modinv(modexp(cipherPair.first, priv, p), p)*cipherPair.second)%p;
        cout<<"("<<cipherPair.first<<", "<<cipherPair.second<<") -> "<<decrypted<<endl;
        return decrypted;
    #endif // DEBUG
    return (modinv(modexp(cipherPair.first, priv, p), p)*cipherPair.second)%p;
}
