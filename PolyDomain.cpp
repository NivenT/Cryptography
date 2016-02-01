#include <algorithm>
#include <cassert>

#include "FiniteField.h"

PolyDomain::PolyDomain(const PrimeField* f) : field(f) {}

const PrimeField* PolyDomain::getField() const {
    return field;
}

Polynomial PolyDomain::makeElement(const BigUnsigned& c) const {
    return makeElement(field->makeElement(c));
}

Polynomial PolyDomain::makeElement(const FieldElement& c) const {
    assert(c.getField() == field);
    return makeElement(std::vector<FieldElement>(1, c));
}

Polynomial PolyDomain::makeElement(const std::vector<FieldElement>& coeffs) const {
    for (auto& c : coeffs) assert(c.getField() == field);
    return Polynomial(coeffs, this);
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

std::ostream& operator<<(std::ostream &os, const PolyDomain& x) {
    return os<<*x.getField()<<"[x]";
}
