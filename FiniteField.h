#ifndef FINITEFIELD_H_INCLUDED
#define FINITEFIELD_H_INCLUDED

#include <BigIntegerLibrary.hh>
#include <vector>

class FieldElement;
class Polynomial;

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

    Polynomial operator()(const BigUnsigned& n) const;
    Polynomial operator()(const FieldElement& n) const;
    Polynomial operator()(const std::initializer_list<FieldElement>& coeffs) const;
};

std::ostream& operator<<(std::ostream &os, const PolyDomain& x);

class Polynomial {
private:
    Polynomial(const std::vector<FieldElement>& coeffs, const PolyDomain* d);

    const PolyDomain* domain;
    std::vector<FieldElement> coeffs;
public:
    const PolyDomain* getDomain() const;
    FieldElement leadingCoefficient() const;
    BigUnsigned getMod() const;
    BigUnsigned degree() const;
    Polynomial trim() const;
    std::size_t size() const;

    FieldElement operator[](unsigned int index) const;
    Polynomial operator+(const Polynomial& rhs) const;
    Polynomial operator-(const Polynomial& rhs) const;
    Polynomial operator*(const FieldElement& rhs) const;
    Polynomial operator*(const Polynomial& rhs) const;
    Polynomial operator%(const Polynomial& rhs) const;
    Polynomial operator-() const;

    friend PolyDomain;
};

std::ostream& operator<<(std::ostream &os, const Polynomial& x);

#endif // FINITEFIELD_H_INCLUDED
