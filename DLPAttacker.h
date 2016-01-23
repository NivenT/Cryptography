#ifndef DLPATTACKER_H_INCLUDED
#define DLPATTACKER_H_INCLUDED

#include <BigIntegerLibrary.hh>

/*
    g^x = h (mod p)
    Given:
        * h
        * p
        * g
    Computer:
        * x
*/
class DLPAttacker {
public:
    static BigUnsigned naive(const BigUnsigned& g, const BigUnsigned& h, const BigUnsigned& p);
    static BigUnsigned babyGiant(const BigUnsigned& g, const BigUnsigned& h, const BigUnsigned& p, const BigUnsigned& order = 0);
};

#endif // DLPATTACKER_H_INCLUDED
