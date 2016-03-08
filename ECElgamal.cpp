//#define DEBUG

#ifdef DEBUG
    #include <iostream>

    using namespace std;
#endif // DEBUG

#include "ECElgamal.h"

ECElgamal::ECElgamal() {}

void ECElgamal::init(unsigned short keyLength) {
    p = CryptoScheme::genPrime(keyLength+1);
    FiniteField* f = new FiniteField(p, 1);
    FFElement a = f->rand(), x = f->rand(), y = f->rand();
    curve = EllipticCurve(f, a, y*y-(x*x*x+a*x)); //b calculated so that (x,y) is on the curve

    priv = CryptoScheme::genCoprime(p-1);
    pubP = curve(x,y);
    pubQ = pubP*priv;

    #ifdef DEBUG
    cout<<"curve: Y^2 = X^3 + ("<<curve.getA()<<")*X + ("<<curve.getB()<<") "<<endl
        <<"p:     "<<p<<" ("<<p.bitLength()<<" bits)"<<endl
        <<"pubP:  "<<pubP<<endl
        <<"pubQ:  "<<pubQ<<endl
        <<"priv:  "<<priv<<" ("<<priv.bitLength()<<" bits)"<<endl
        <<endl;
    #endif // DEBUG
}

BigUnsigned ECElgamal::getModulus() const {
    return p;
}

ECEpair ECElgamal::encrypt(const BigUnsigned& msg, const BigUnsigned& k) const {
    return std::make_pair(pubP*k, (pubQ*k).getX()*curve.getField()->makeElement(msg));
}

void* ECElgamal::encrypt(const BigUnsigned& msg) const {
    #ifdef DEBUG
        ECEpair cipher = encrypt(msg, CryptoScheme::random(priv.bitLength()));
        cout<<msg<<" -> ("<<cipher.first<<", "<<cipher.second<<")"<<" (encryption)"<<endl;
        return new ECEpair(cipher);
    #else
        return new ECEpair(encrypt(msg, CryptoScheme::random(priv.bitLength())));
    #endif // DEBUG
}

BigUnsigned ECElgamal::decrypt(void* cipher) const {
    ECEpair cipherPair = *(ECEpair*)cipher;
    #ifdef DEBUG
        BigUnsigned decrypted = (cipherPair.second/(cipherPair.first*priv).getX()).getVal()[0].getVal();
        cout<<"("<<cipherPair.first<<", "<<cipherPair.second<<") -> "<<decrypted<<" (decryption)"<<endl;
        return decrypted;
    #else
        return (cipherPair.second/(cipherPair.first*priv).getX()).getVal()[0].getVal();
    #endif // DEBUG
}
