#include <algorithm>
#include <cassert>

#include "FiniteField.h"

Polynomial::Polynomial(const std::vector<FieldElement>& coeffs, const PolyDomain* d) : coeffs(coeffs), domain(d) {}

Polynomial::Polynomial() : coeffs(), domain(nullptr) {}

Polynomial::~Polynomial() {coeffs.clear();}

std::vector<FieldElement> Polynomial::getCoeffs() const {
    return coeffs;
}

const PolyDomain* Polynomial::getDomain() const {
    return domain;
}

BigUnsigned Polynomial::degree() const {
    return coeffs.size() == 0 ? (int)0 : (int)coeffs.size()-1;
}

BigUnsigned Polynomial::getMod() const {
    return domain->getField()->getMod();
}

FieldElement Polynomial::leadingCoefficient() const {
    return coeffs.size() == 0 ? domain->getField()->makeElement(0) : coeffs.back();
}

Polynomial Polynomial::trim() const {
    if (coeffs.size() == 0) return *this;
    Polynomial res = *this;
    FieldElement zero = res.domain->getField()->makeElement(0);
    while (res.coeffs.back() == zero) res.coeffs.pop_back();
    return res;
}

std::size_t Polynomial::size() const {
    return coeffs.size();
}

std::tuple<Polynomial, Polynomial> Polynomial::divmod(const Polynomial& rhs) const {
    Polynomial remainder = *this, divisor = rhs.trim(), quotient = domain->makeElement(0);
    while (remainder.degree() >= divisor.degree() && remainder != 0) {
        BigUnsigned expnt = remainder.degree() - divisor.degree();
        assert(divisor.leadingCoefficient().getVal() != 0);
        FieldElement coeff = remainder.leadingCoefficient()/divisor.leadingCoefficient();

        auto vec = std::vector<FieldElement>(expnt.toUnsignedLong(),domain->getField()->makeElement(0));
        vec.push_back(coeff);
        Polynomial mono(vec, domain);
        mono = mono.trim();

        quotient = quotient + mono;
        remainder = remainder - mono*divisor;
    }
    return std::make_tuple(quotient.trim(), remainder.trim());
}

FieldElement Polynomial::operator[](unsigned int index) const {
    return coeffs[index];
}

Polynomial Polynomial::operator+(const Polynomial& rhs) const {
    assert(domain == rhs.domain);
    std::vector<FieldElement> c;
    int s = std::min(coeffs.size(), rhs.coeffs.size());
    for (int i = 0; i < s; i++) {
        c.push_back(coeffs[i]+rhs.coeffs[i]);
    }
    c.insert(c.end(), coeffs.begin()+s, coeffs.end());
    c.insert(c.end(), rhs.coeffs.begin()+s, rhs.coeffs.end());
    return Polynomial(c, domain).trim();
}

Polynomial Polynomial::operator-(const Polynomial& rhs) const {
    assert(domain == rhs.domain);
    std::vector<FieldElement> c;
    int s = std::min(coeffs.size(), rhs.coeffs.size());
    for (int i = 0; i < s; i++) {
        c.push_back(coeffs[i]-rhs.coeffs[i]);
    }
    c.insert(c.end(), coeffs.begin()+s, coeffs.end());
    c.resize(c.size()+rhs.coeffs.size()-s);
    std::transform(rhs.coeffs.begin()+s, rhs.coeffs.end(), c.begin()+coeffs.size(), std::negate<FieldElement>());
    return Polynomial(c, domain).trim();
}

Polynomial Polynomial::operator-() const {
    std::vector<FieldElement> c(coeffs.size());
    std::transform(coeffs.begin(), coeffs.end(), c.begin(), std::negate<FieldElement>());
    return Polynomial(c, domain);
}

Polynomial Polynomial::operator*(const FieldElement& rhs) const {
    assert(domain->getField() == rhs.getField());
    std::vector<FieldElement> c(coeffs.size(), rhs);
    std::transform(coeffs.begin(), coeffs.end(), c.begin(), c.begin(), std::multiplies<FieldElement>());
    return Polynomial(c, domain).trim();
}

Polynomial Polynomial::operator*(const Polynomial& rhs) const {
    assert(domain == rhs.domain);
    FieldElement zero = domain->getField()->makeElement(0);
    Polynomial res(std::vector<FieldElement>(), domain);
    for (int i = 0; i < coeffs.size(); i++) {
        if (coeffs[i] == zero) continue;
        std::vector<FieldElement> temp(rhs.coeffs.begin(), rhs.coeffs.end());
        temp.insert(temp.begin(), i, domain->getField()->makeElement(0));
        res = res + Polynomial(temp, domain) * coeffs[i];
        temp.clear();
    }
    return res;
}

Polynomial Polynomial::operator/(const Polynomial& rhs) const {
    return std::get<0>(divmod(rhs));
}

Polynomial Polynomial::operator%(const Polynomial& rhs) const {
    return std::get<1>(divmod(rhs));
}

bool Polynomial::operator==(const BigUnsigned& rhs) const {
    return coeffs.size() == 1 ? coeffs.back().getVal() == rhs : (coeffs.size() == 0 && rhs == 0);
}

bool Polynomial::operator!=(const BigUnsigned& rhs) const {
    return coeffs.size() == 1 ? coeffs.back().getVal() != rhs : (coeffs.size() != 0 || rhs != 0);
}

bool Polynomial::operator==(const Polynomial& rhs) const {
    if (coeffs.size() != rhs.coeffs.size()) return false;
    return equal(coeffs.begin(), coeffs.end(), rhs.coeffs.begin());
}

bool Polynomial::operator<(const Polynomial& rhs) const {
    return coeffs.size() < rhs.coeffs.size();
}

BigUnsigned pow(BigUnsigned lhs, BigUnsigned rhs) {
    if (rhs == 0) return 1;
    else if (rhs == 1) return lhs;
    else return rhs%2 == 0 ? pow(lhs*lhs, rhs/2) : pow(lhs*lhs, rhs/2)*lhs;
}

Polynomial Polynomial::powmod(const BigUnsigned& rhs, const Polynomial& modulus) const {
    Polynomial res = domain->makeElement(1);
    Polynomial base = (*this)%modulus;
    BigUnsigned n = 1;
    while (rhs > n) {
        if ((rhs & n) == n) {
            res = (res*base)%modulus;
        }
        n <<= 1;
        base = (base*base)%modulus;
    }
    return res;
}

bool Polynomial::isIrreducible() const {
    Polynomial x = (*domain)({domain->getField()->makeElement(0),domain->getField()->makeElement(1)});
    Polynomial xPower = x;
    for (BigUnsigned k = 0; k <= degree()/2; k++) {
        xPower = xPower.powmod(getMod(), *this);
        if (gcd(xPower-x, *this).degree() > 0) return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream &os, const Polynomial& x) {
    if (x.size() == 0) return os<<"0 "<<(char)238<<" "<<*x.getDomain();
    if (x[0].getVal() != 0) os<<x[0].getVal();
    for (int n = 1, k = 1; n < x.size(); n++) {
        if (x[n].getVal() == 0) continue;
        if (x[0].getVal() != 0 || k++ > 1) os<<" + ";
        if (x[n].getVal() != 1) os<<x[n].getVal();
        os<<"x";
        if (n != 1) os<<"^"<<n;
    }
    os<<" "<<(char)238<<" "<<*x.getDomain();
    return os;
}
