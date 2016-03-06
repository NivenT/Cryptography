#include <cassert>

#include "FiniteField.h"

FiniteField::FiniteField(const BigUnsigned& p, const BigUnsigned& k) : p(p), k(k) {
    assert(k > 1 && "Use PrimeField if k==1");
    domain = new PolyDomain(new PrimeField(p));
    modulus = domain->generateIrreducible(k);
}

PolyDomain* FiniteField::getDomain() const {
    return domain;
}

BigUnsigned FiniteField::getP() const {
    return p;
}

BigUnsigned FiniteField::getK() const {
    return k;
}

Polynomial FiniteField::getMod() const {
    return modulus;
}

FFElement FiniteField::makeElement(const Polynomial& p) const {
    assert(p.getMod() == getP());
    if (p.getDomain() == domain) return FFElement(p, this);
    std::vector<FieldElement> coeffs;
    for (auto& c : p.getCoeffs()) coeffs.push_back(domain->getField()->makeElement(c.getVal()));
    return FFElement(domain->makeElement(coeffs), this);
}

FFElement FiniteField::makeElement(const BigUnsigned& c) const {
    return makeElement(domain->makeElement(c));
}

FFElement FiniteField::makeElement(const FieldElement& c) const {
    return makeElement(domain->makeElement(c));
}

FFElement FiniteField::makeElement(const std::vector<FieldElement>& vec) const {
    return makeElement(domain->makeElement(vec));
}

FFElement FiniteField::operator()(const Polynomial& p) const {
    return makeElement(p);
}

FFElement FiniteField::operator()(const BigUnsigned& c) const {
    return makeElement(domain->makeElement(c));
}

FFElement FiniteField::operator()(const FieldElement& c) const {
    return makeElement(domain->makeElement(c));
}

FFElement FiniteField::operator()(const std::initializer_list<FieldElement>& coeffs) const {
    return makeElement(std::vector<FieldElement>(coeffs.begin(), coeffs.end()));
}

std::ostream& operator<<(std::ostream &os, const FiniteField& x) {
    return os<<"F_{"<<x.getP()<<"^"<<x.getK()<<"}";
}