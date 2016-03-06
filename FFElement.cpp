#include <cassert>

#include "FiniteField.h"

FFElement::FFElement(const Polynomial& poly, const FiniteField* f) : val(poly%f->getMod()), field(f) {}

const FiniteField* FFElement::getField() const {
    return field;
}

Polynomial FFElement::getVal() const {
    return val;
}

FFElement FFElement::inverse() const {
    Polynomial  r0 = field->getMod(), r1 = val;
    //Polynomial  s0 = field->getDomain()->makeElement(1), s1 = field->getDomain()->makeElement(0);
    Polynomial  t0 = field->getDomain()->makeElement(0), t1 = field->getDomain()->makeElement(1);
    while (r1 != 0) {
        Polynomial q = r0/r1;
        std::tie(r0, r1) = std::make_tuple(r1,r0-q*r1);
        //std::tie(s0, s1) = std::make_tuple(s1,s0-q*s1);
        std::tie(t0, t1) = std::make_tuple(t1,t0-q*t1);
    }
    return field->makeElement(t0*r0[0].inverse());
}

FFElement FFElement::operator+(const FFElement& rhs) const {
    assert(field == rhs.field);
    return FFElement(val+rhs.val, field);
}

FFElement FFElement::operator-(const FFElement& rhs) const {
    assert(field == rhs.field);
    return FFElement(val-rhs.val, field);
}

FFElement FFElement::operator*(const FFElement& rhs) const {
    assert(field == rhs.field);
    return FFElement(val*rhs.val, field);
}

FFElement FFElement::operator/(const FFElement& rhs) const {
    assert(field == rhs.field);
    return FFElement(val*rhs.inverse().val, field);
}

FFElement FFElement::operator-() const {
    return FFElement(-val, field);
}

bool FFElement::operator==(const FFElement& rhs) const {
    assert(field == rhs.field);
    return val == rhs.val;
}

bool FFElement::operator!=(const FFElement& rhs) const {
    assert(field == rhs.field);
    return val != rhs.val;
}

std::ostream& operator<<(std::ostream &os, const FFElement& x) {
    if (x.getVal().size() == 0) return os<<"0 "<<(char)238<<" "<<*x.getField();
    if (x.getVal()[0].getVal() != 0) os<<x.getVal()[0].getVal();
    for (int n = 1, k = 1; n < x.getVal().size(); n++) {
        if (x.getVal()[n].getVal() == 0) continue;
        if (x.getVal()[0].getVal() != 0 || k++ > 1) os<<" + ";
        if (x.getVal()[n].getVal() != 1) os<<x.getVal()[n].getVal();
        os<<"x";
        if (n != 1) os<<"^"<<n;
    }
    os<<" "<<(char)238<<" "<<*x.getField();
    return os;
}
