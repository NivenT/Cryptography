//#define DEBUG

#ifdef DEBUG
    #include <iostream>

    using namespace std;
#endif // DEBUG

#include "RSA.h"

RSA::RSA() {
}

BigUnsigned RSA::getModulus() const {
    return n;
}

void RSA::init(unsigned short keyLength) {
    BigUnsigned p = CryptoScheme::genPrime(keyLength/2), q = CryptoScheme::genPrime(keyLength/2);
    BigUnsigned phi = (p-1)*(q-1);
    priv = CryptoScheme::genCoprime(phi);
    this->keyLength = priv.bitLength();

    n = p*q;
    pub = modinv(priv, phi);

    #ifdef DEBUG
    cout<<"p:          "<<p<<" ("<<p.bitLength()<<" bits)"<<endl
        <<"q:          "<<q<<" ("<<q.bitLength()<<" bits)"<<endl
        <<"(p-1)(q-1): "<<phi<<" ("<<phi.bitLength()<<" bits)"<<endl
        <<"priv:       "<<priv<<" ("<<priv.bitLength()<<" bits)"<<endl
        <<"pub:        "<<pub<<" ("<<pub.bitLength()<<" bits)"<<endl
        <<"n:          "<<n<<" ("<<n.bitLength()<<" bits)"<<endl
        <<endl;
    #endif // DEBUG
}

void* RSA::encrypt(const BigUnsigned& msg) const {
    #ifdef DEBUG
        BigUnsigned cipher = modexp(msg, pub, n);
        cout<<msg<<" -> "<<cipher<<" (encryption)"<<endl;
        return new BigUnsigned(cipher);
    #else
        return new BigUnsigned(modexp(msg, pub, n));
    #endif // DEBUG
}

BigUnsigned RSA::decrypt(void* cipher) const {
    #ifdef DEBUG
        BigUnsigned c = *(BigUnsigned*)cipher;
        BigUnsigned decrypted = modexp(c, priv, n);
        cout<<c<<" -> "<<decrypted<<" (decryption)"<<endl;
        return decrypted;
    #else
        return modexp(*(BigUnsigned*)cipher, priv, n);
    #endif // DEBUG
}
