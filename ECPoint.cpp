#include <cassert>

#include "EllipticCurve.h"

ECPoint::ECPoint(const FFElement& x, const FFElement& y, const EllipticCurve* c, bool ideal) : x(x), y(y), curve(c), ideal(ideal) {
    assert(x.getField() == c->getField() && y.getField() == c->getField());
    assert(ideal || y*y == x*x*x + c->getA()*x + c->getB());
}

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
