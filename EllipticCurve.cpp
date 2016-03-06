#include <cassert>

#include "EllipticCurve.h"

EllipticCurve::EllipticCurve(const FiniteField* f, const FFElement& a, const FFElement& b) : field(f), a(a), b(b) {
    assert(a.getField() == f && b.getField() == f);
}

const FiniteField* EllipticCurve::getField() const {
    return field;
}

FFElement EllipticCurve::getA() const {
    return a;
}

FFElement EllipticCurve::getB() const {
    return b;
}

bool EllipticCurve::onCurve(const FFElement& x, const FFElement& y) const {
    return y*y == x*x*x + a*x + b;
}

ECPoint EllipticCurve::makeElement(const FFElement& x, const FFElement& y) const {
    return ECPoint(x, y, this);
}

ECPoint EllipticCurve::makeElement() const {
    return ECPoint(field->makeElement(0), field->makeElement(0), this, true);
}

ECPoint EllipticCurve::operator()(const FFElement& x, const FFElement& y) const {
    return ECPoint(x, y, this);
}

ECPoint EllipticCurve::operator()() const {
    return makeElement();
}

std::ostream& operator<<(std::ostream &os, const EllipticCurve& x) {
    return os<<"E("<<*x.getField()<<")";
}
