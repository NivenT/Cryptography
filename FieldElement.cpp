#include <cassert>

#include "FiniteField.h"

FieldElement::FieldElement(const BigUnsigned& n, const PrimeField* f) : n(n%f->getMod()), field(f) {}

BigUnsigned FieldElement::getMod() const {
    return field->getMod();
}

BigUnsigned FieldElement::getVal() const {
    return n;
}

const PrimeField* FieldElement::getField() const {
    return field;
}

FieldElement FieldElement::inverse() const {
    return FieldElement(modinv(n, getMod()), field);
}

FieldElement FieldElement::operator+(const FieldElement& rhs) const {
    assert(field == rhs.field);
    return FieldElement(n+rhs.n, field);
}

FieldElement FieldElement::operator-(const FieldElement& rhs) const {
    assert(field == rhs.field);
    return FieldElement(n+(getMod()-rhs.n), field);
}

FieldElement FieldElement::operator*(const FieldElement& rhs) const {
    assert(field == rhs.field);
    return FieldElement(n*rhs.n, field);
}

FieldElement FieldElement::operator/(const FieldElement& rhs) const {
    assert(field == rhs.field);
    return FieldElement(n*modinv(rhs.n, getMod()), field);
}

FieldElement FieldElement::operator-() const {
    return FieldElement(getMod()-n, field);
}

std::ostream& operator<<(std::ostream &os, const FieldElement& x) {
    return os<<x.getVal()<<" (mod "<<x.getMod()<<")";
}
