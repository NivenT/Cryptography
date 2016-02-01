#ifndef INTEGERFACTORER_H_INCLUDED
#define INTEGERFACTORER_H_INCLUDED

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

#endif // INTEGERFACTORER_H_INCLUDED
