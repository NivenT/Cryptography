#ifndef ECDLPATTACKER_H_INCLUDED
#define ECDLPATTACKER_H_INCLUDED

#include "EllipticCurve.h"

/*
    nP = Q
    Given:
        * P
        * Q
    Find:
        * n
*/

class ECDLPAttacker {
public:
    static BigUnsigned naive(const ECPoint& p, const ECPoint& q);
    static BigUnsigned babyGiant(const ECPoint& p, const ECPoint& q, const BigUnsigned& order = 0);
};

#endif // ECDLPATTACKER_H_INCLUDED
