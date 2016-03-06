#ifndef ELLIPTICCURVE_H_INCLUDED
#define ELLIPTICCURVE_H_INCLUDED

#include "FiniteField.h"

class ECPoint;

class EllipticCurve {
private:
    const FiniteField* field;
    FFElement a, b; /// y^2 = x^3 + ax + b
public:
    EllipticCurve(const FiniteField* f, const FFElement& a, const FFElement& b);
    const FiniteField* getField() const;

    ECPoint makeElement(const FFElement& x, const FFElement& y) const;
    ECPoint operator()(const FFElement& x, const FFElement& y) const;
};

std::ostream& operator<<(std::ostream &os, const EllipticCurve& x);

class ECPoint {
private:
    ECPoint(const FFElement& x, const FFElement& y, const EllipticCurve* c, bool ideal = false);

    const EllipticCurve* curve;
    FFElement x, y;
    bool isIdeal;
public:
    ECPoint operator+(const ECPoint& rhs) const;
    ECPoint operator-(const ECPoint& rhs) const;
    ECPoint operator*(const BigUnsigned& rhs) const;
    ECPoint operator-() const;

    bool operator==(const ECPoint& rhs) const;
    bool operator!=(const ECPoint& rhs) const;

    friend EllipticCurve;
};

std::ostream& operator<<(std::ostream &os, const ECPoint& x);

#endif // ELIPTICCURVE_H_INCLUDED
