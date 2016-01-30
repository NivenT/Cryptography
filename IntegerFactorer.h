#ifndef INTEGERFACTORIZER_H_INCLUDED
#define INTEGERFACTORIZER_H_INCLUDED

#include <BigIntegerLibrary.hh>

/*
    n = p^a * q^b * ...
    Given:
        * n
    Compute:
        * p (in other words, returns a single prime factor of n)
*/
class IntFactorer {
public:
    static BigUnsigned naive(const BigUnsigned& n);
    static BigUnsigned pollard(const BigUnsigned& n); // n=p*q
};

#endif // INTEGERFACTORIZER_H_INCLUDED
