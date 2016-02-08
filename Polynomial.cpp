#include <algorithm>
#include <cassert>

#include "FiniteField.h"

Polynomial::Polynomial(const std::vector<FieldElement>& coeffs, const PolyDomain* d) : coeffs(coeffs), domain(d) {}

const PolyDomain* Polynomial::getDomain() const {
    return domain;
}

BigUnsigned Polynomial::degree() const {
    return coeffs.size() == 0 ? (int)0 : (int)coeffs.size()-1;
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
    Polynomial res(std::vector<FieldElement>(), domain);
    for (int i = 0; i < coeffs.size(); i++) {
        std::vector<FieldElement> temp(rhs.coeffs.begin(), rhs.coeffs.end());
        temp.insert(temp.begin(), i, domain->getField()->makeElement(0));
        res = res + Polynomial(temp, domain) * coeffs[i];
        temp.clear();
    }
    return res;
}

Polynomial Polynomial::operator%(const Polynomial& rhs) const {
    Polynomial remainder = *this;
    while (remainder.degree() >= rhs.degree()) {
        BigUnsigned expnt = remainder.degree() - rhs.degree();
        FieldElement coeff = remainder.leadingCoefficient()/rhs.leadingCoefficient();

        auto vec = std::vector<FieldElement>(expnt.toUnsignedLong(),domain->getField()->makeElement(0));
        vec.push_back(coeff);
        Polynomial mono(vec, domain);

        remainder = remainder - mono*rhs;
    }
    return remainder;
}

std::ostream& operator<<(std::ostream &os, const Polynomial& x) {
    if (x.size() == 0) return os<<"0 "<<(char)238<<" "<<*x.getDomain();
    os<<x[0].getVal();
    for (int n = 1; n < x.size(); n++) {
        if (x[n].getVal() == 0) continue;
        os<<" + "<<x[n].getVal()<<"x";
        if (n != 1) os<<"^"<<n;
    }
    os<<" "<<(char)238<<" "<<*x.getDomain();
    return os;
}
