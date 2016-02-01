#include "FiniteField.h"

Polynomial::Polynomial(const std::vector<FieldElement>& coeffs, const PolyDomain* d) : coeffs(coeffs), domain(d) {}

const PolyDomain* Polynomial::getDomain() const {
    return domain;
}

FieldElement Polynomial::operator[](unsigned int index) const {
    return coeffs[index];
}

std::size_t Polynomial::size() const {
    return coeffs.size();
}

std::ostream& operator<<(std::ostream &os, const Polynomial& x) {
    if (x.size() == 0) return os<<"0";
    os<<x[0].getVal();
    for (int n = 1; n < x.size(); n++) {
        os<<" + "<<x[n].getVal()<<"x";
        if (n != 1) os<<"^"<<n;
    }
    os<<" "<<(char)238<<" "<<*x.getDomain();
    return os;
}
