#include <chrono>

#include "CryptoScheme.h"

CryptoScheme::CryptoScheme() {
}

std::vector<BigUnsigned> CryptoScheme::encode(const std::string& msg, const BigUnsigned& modulus) {
    std::vector<BigUnsigned> encoding(msg.size());
    for (int i = 0; i < msg.size(); i++) {
        encoding[i] = msg[i];
    }
    return encoding;
}

std::string CryptoScheme::decode(const std::vector<BigUnsigned>& enc, const BigUnsigned& modulus) {
    std::string res = "";
    for (auto& e : enc) {
        int c = e.toInt();
        res += (char)c;
    }
    return res;
}

BigUnsigned CryptoScheme::random(unsigned short numBits) {
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    BigUnsigned res = 0, summand = 1;
    for (int i = 1; i < numBits; i++) {
        if (rng()%2) res += summand;
        summand *= 2;
    }
    return res + summand;
}

BigUnsigned CryptoScheme::genPrime(unsigned short numBits) {
    BigUnsigned prime = random(numBits);
    while (!isProbablyPrime(prime)) {
        prime = random(numBits);
    }
    return prime;
}

BigUnsigned CryptoScheme::findGenerator(const BigUnsigned& n) {
    BigUnsigned gen = random(n.bitLength());
    while(gcd(gen, n) != 1) {
        gen = random(n.bitLength());
    }
    return gen;
}

BigUnsigned CryptoScheme::findOrder(const BigUnsigned& n, const BigUnsigned& modulus) {
    BigUnsigned a = n*n;
    for (BigUnsigned order = 2;; order++) {
        if (a == 1) return order;
        a = (a*n)%modulus;
    }
}

bool CryptoScheme::isProbablyPrime(const BigUnsigned& n) {
    for (int i = 0; i < 30; i++) {
        BigUnsigned a = random(n.bitLength());
        if (modexp(a, n-1, n) != 1) return false;
    }
    return true;
}
