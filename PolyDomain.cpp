#include <algorithm>
#include <cassert>

#include "CryptoScheme.h"
#include "FiniteField.h"

PolyDomain::PolyDomain(const PrimeField* f) : field(f) {}

const PrimeField* PolyDomain::getField() const {
    return field;
}

Polynomial PolyDomain::makeElement(const BigUnsigned& c) const {
    return makeElement(field->makeElement(c));
}

Polynomial PolyDomain::makeElement(const FieldElement& c) const {
    /*
    assert(c.getMod() == field->getMod());
    if (c.getField() == field) return makeElement(std::vector<FieldElement>(1,c));
    return makeElement(std::vector<FieldElement>(1, field->makeElement(c.getVal())));
    */
    assert(c.getField() == field);
    return makeElement(std::vector<FieldElement>(1,c));
}

Polynomial PolyDomain::makeElement(const std::vector<FieldElement>& coeffs) const {
    for (auto& c : coeffs) assert(c.getField() == field);
    return Polynomial(coeffs, this);
}

Polynomial PolyDomain::randomPoly(const BigUnsigned& degree) const {
    std::vector<FieldElement> coefficients;
    for (BigUnsigned i = 0; i <= degree; i++) {
        coefficients.push_back(field->makeElement(CryptoScheme::random(field->getMod().bitLength())));
    }
    return makeElement(coefficients);
}

Polynomial PolyDomain::generateIrreducible(const BigUnsigned& degree) const {
    Polynomial poly = randomPoly(degree);
    while (!poly.isIrreducible()) poly = randomPoly(degree);
    return poly;
}

Polynomial PolyDomain::operator()(const BigUnsigned& c) const {
    return makeElement(field->makeElement(c));
}

Polynomial PolyDomain::operator()(const FieldElement& c) const {
    return makeElement(c);
}

Polynomial PolyDomain::operator()(const std::initializer_list<FieldElement>& c) const {
    return makeElement(std::vector<FieldElement>(c.begin(), c.end()));
}

Polynomial PolyDomain::operator()(const std::initializer_list<BigUnsigned>& c) const {
    std::vector<FieldElement> coeffs(c.size());
    std::transform(c.begin(), c.end(), coeffs.begin(), [&](const BigUnsigned& b) {
                        return field->makeElement(b);
                   });
    return makeElement(coeffs);
}

std::ostream& operator<<(std::ostream &os, const PolyDomain& x) {
    return os<<*x.getField()<<"[x]";
}
