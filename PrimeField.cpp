#include <cassert>

#include "CryptoScheme.h"
#include "FiniteField.h"

PrimeField::PrimeField(const BigUnsigned& p) : p(p) {
    assert(CryptoScheme::isProbablyPrime(p));
}

FieldElement PrimeField::makeElement(const BigUnsigned& n) const {
    return FieldElement(n, this);
}

FieldElement PrimeField::operator()(const BigUnsigned& n) const {
    return FieldElement(n, this);
}

BigUnsigned PrimeField::getMod() const {
    return p;
}

std::ostream& operator<<(std::ostream &os, const PrimeField& x) {
    return os<<"F_"<<x.getMod();
}
