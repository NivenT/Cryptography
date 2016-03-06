#ifndef FINITEFIELD_H_INCLUDED
#define FINITEFIELD_H_INCLUDED

#include <BigIntegerLibrary.hh>
#include <vector>
#include <tuple>

class FieldElement;
class Polynomial;
class FFElement;

class PrimeField {
private:
    BigUnsigned p;
public:
    PrimeField(const BigUnsigned& p);
    FieldElement makeElement(const BigUnsigned& n) const;
    BigUnsigned getMod() const;

    FieldElement operator()(const BigUnsigned& n) const;
};

std::ostream& operator<<(std::ostream &os, const PrimeField& x);

class FieldElement {
private:
    FieldElement(const BigUnsigned& n, const PrimeField* f);

    const PrimeField* field;
    BigUnsigned n;
public:
    FieldElement();
    const PrimeField* getField() const;
    BigUnsigned getMod() const;
    BigUnsigned getVal() const;
    FieldElement inverse() const;

    FieldElement operator+(const FieldElement& rhs) const;
    FieldElement operator-(const FieldElement& rhs) const;
    FieldElement operator*(const FieldElement& rhs) const;
    FieldElement operator/(const FieldElement& rhs) const;
    FieldElement operator-() const;

    bool operator==(const FieldElement& rhs) const;

    friend PrimeField;
};

std::ostream& operator<<(std::ostream &os, const FieldElement& x);

class PolyDomain {
private:
    const PrimeField* field;
public:
    PolyDomain(const PrimeField* f);
    const PrimeField* getField() const;
    Polynomial makeElement(const BigUnsigned& c) const;
    Polynomial makeElement(const FieldElement& c) const;
    Polynomial makeElement(const std::vector<FieldElement>& coeffs) const;
    Polynomial randomPoly(const BigUnsigned& degree) const;
    Polynomial generateIrreducible(const BigUnsigned& degree) const;

    Polynomial operator()(const BigUnsigned& n) const;
    Polynomial operator()(const FieldElement& n) const;
    Polynomial operator()(const std::initializer_list<FieldElement>& coeffs) const;
    Polynomial operator()(const std::initializer_list<BigUnsigned>& coeffs) const;
};

std::ostream& operator<<(std::ostream &os, const PolyDomain& x);

class Polynomial {
private:
    Polynomial(const std::vector<FieldElement>& coeffs, const PolyDomain* d);
    std::tuple<Polynomial, Polynomial> divmod(const Polynomial& rhs) const;

    const PolyDomain* domain;
    std::vector<FieldElement> coeffs;
public:
    Polynomial();
    ~Polynomial();
    std::vector<FieldElement> getCoeffs() const;
    const PolyDomain* getDomain() const;
    FieldElement leadingCoefficient() const;
    BigUnsigned getMod() const;
    BigUnsigned degree() const;
    Polynomial trim() const;
    std::size_t size() const;

    Polynomial powmod(const BigUnsigned& rhs, const Polynomial& modulus) const;
    bool isIrreducible() const;

    FieldElement operator[](unsigned int index) const;
    Polynomial operator+(const Polynomial& rhs) const;
    Polynomial operator-(const Polynomial& rhs) const;
    Polynomial operator*(const FieldElement& rhs) const;
    Polynomial operator*(const Polynomial& rhs) const;
    /// this = rhs*q + r
    Polynomial operator/(const Polynomial& rhs) const; //returns q
    Polynomial operator%(const Polynomial& rhs) const; //returns r
    Polynomial operator-() const;

    bool operator==(const BigUnsigned& rhs) const;
    bool operator!=(const BigUnsigned& rhs) const;

    bool operator==(const Polynomial& rhs) const;
    bool operator!=(const Polynomial& rhs) const;
    bool operator<(const Polynomial& rhs) const;

    friend PolyDomain;
};

std::ostream& operator<<(std::ostream &os, const Polynomial& x);

class FiniteField {
private:
    const PolyDomain* domain;
    BigUnsigned p, k;
    Polynomial modulus;
public:
    FiniteField(const BigUnsigned& p, const BigUnsigned& k);
    FiniteField(const PrimeField* f, const BigUnsigned& k);
    FiniteField(const PolyDomain* d, const BigUnsigned& k);
    FiniteField(const Polynomial& irred);
    const PolyDomain* getDomain() const;
    BigUnsigned getP() const;
    BigUnsigned getK() const;
    Polynomial getMod() const;

    FFElement makeElement(const Polynomial& p) const;
    FFElement makeElement(const BigUnsigned& c) const;
    FFElement makeElement(const FieldElement& c) const;
    FFElement makeElement(const std::vector<FieldElement>& coeffs) const;
    FFElement rand() const;

    FFElement operator()(const Polynomial& p) const;
    FFElement operator()(const BigUnsigned& n) const;
    FFElement operator()(const FieldElement& n) const;
    FFElement operator()(const std::initializer_list<FieldElement>& coeffs) const;
    FFElement operator()(const std::initializer_list<BigUnsigned>& coeffs) const;
};

std::ostream& operator<<(std::ostream &os, const FiniteField& x);

class FFElement {
private:
    FFElement(const Polynomial& poly, const FiniteField* f);

    const FiniteField* field;
    Polynomial val;
public:
    const FiniteField* getField() const;
    Polynomial getVal() const;
    FFElement inverse() const;

    FFElement operator+(const FFElement& rhs) const;
    FFElement operator-(const FFElement& rhs) const;
    FFElement operator*(const FFElement& rhs) const;
    FFElement operator/(const FFElement& rhs) const;
    FFElement operator-() const;

    bool operator==(const FFElement& rhs) const;
    bool operator!=(const FFElement& rhs) const;

    friend FiniteField;
};

std::ostream& operator<<(std::ostream &os, const FFElement& x);

template<class T>
T gcd(T lhs, T rhs) {
    if (lhs < rhs) return gcd(rhs, lhs);
    else if (rhs == 0) return lhs;
    else return gcd(rhs, lhs%rhs);
}

#endif // FINITEFIELD_H_INCLUDED
