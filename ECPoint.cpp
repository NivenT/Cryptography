#include <cassert>

#include "EllipticCurve.h"

ECPoint::ECPoint(const FFElement& x, const FFElement& y, const EllipticCurve* c, bool ideal) : x(x), y(y), curve(c), ideal(ideal) {
    assert(x.getField() == c->getField() && y.getField() == c->getField());
    assert(ideal || y*y == x*x*x + c->getA()*x + c->getB());
}

ECPoint::ECPoint(const EllipticCurve* c) : x(c->getField()->makeElement(0)), y(c->getField()->makeElement(0)), curve(c), ideal(true) {}

ECPoint::ECPoint() : curve(nullptr), ideal(true) {}

const EllipticCurve* ECPoint::getCurve() const {
    return curve;
}

FFElement ECPoint::getX() const {
    return x;
}

FFElement ECPoint::getY() const {
    return y;
}

bool ECPoint::isIdeal() const {
    return ideal;
}

ECPoint ECPoint::operator+(const ECPoint& rhs) const {
    assert(curve == rhs.curve);
    if (ideal) return rhs;
    else if (rhs.ideal) return *this;
    else if (x == rhs.x) {
        if (y == -rhs.y) return ECPoint(curve);
        FFElement slope = (x*x*curve->getField()->makeElement(3) + curve->getA())/(y+y);
        FFElement x3 = slope*slope - x - x;

        return ECPoint(x3, -(slope*(x3-x)+y), curve);
    } else {
        FFElement slope = (rhs.y-y)/(rhs.x-x);
        FFElement x3 = slope*slope - x - rhs.x;

        return ECPoint(x3, -(slope*(x3-x)+y), curve);
    }
}

ECPoint ECPoint::operator-() const {
    return ideal ? *this : ECPoint(x, -y, curve);
}

ECPoint ECPoint::operator-(const ECPoint& rhs) const {
    assert(curve == rhs.curve);
    return *this + (-rhs);
}

ECPoint ECPoint::operator*(const BigUnsigned& rhs) const {
    if (rhs == 0 || ideal) return ECPoint(curve);
    ECPoint res = ECPoint(curve);
    ECPoint summand = *this;
    BigUnsigned n = 1;
    while (rhs > n) {
        res = (rhs & n) > 0 ? res + summand : res;
        summand = summand+summand;
        n <<= 1;
    }
    return res;
}

bool ECPoint::operator==(const ECPoint& rhs) const {
    assert(curve == rhs.curve);
    return (ideal || rhs.ideal) ? (ideal && rhs.ideal) : x == rhs.x && y == rhs.y;
}

bool ECPoint::operator!=(const ECPoint& rhs) const {
    assert(curve == rhs.curve);
    return (ideal || rhs.ideal) ? (!ideal || !rhs.ideal) : x != rhs.x || y != rhs.y;
}

void printCoord(std::ostream &os, const FFElement& x) {
    if (x.getVal()[0].getVal() != 0) os<<x.getVal()[0].getVal();
    for (int n = 1, k = 1; n < x.getVal().size(); n++) {
        if (x.getVal()[n].getVal() == 0) continue;
        if (x.getVal()[0].getVal() != 0 || k++ > 1) os<<" + ";
        if (x.getVal()[n].getVal() != 1) os<<x.getVal()[n].getVal();
        os<<"x";
        if (n != 1) os<<"^"<<n;
    }
}

std::ostream& operator<<(std::ostream &os, const ECPoint& x) {
    if (x.isIdeal()) return os<<"IDEAL";
    os<<"(";
    printCoord(os, x.getX());
    os<<", ";
    printCoord(os, x.getY());
    return os<<") "<<(char)238<<" "<<*x.getCurve();
}
