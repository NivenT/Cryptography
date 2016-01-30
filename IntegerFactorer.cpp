#include "CryptoScheme.h"
#include "IntegerFactorer.h"

BigUnsigned IntFactorer::naive(const BigUnsigned& n) {
    for (BigUnsigned p = 2; p < n; p++) {
        if (n%p != 0) continue;
        if (CryptoScheme::isProbablyPrime(p)) return p;
    }
    return n;
}

BigUnsigned IntFactorer::pollard(const BigUnsigned& n) {
    for (BigUnsigned a = 2;; a++) {
        BigUnsigned anfact = a;
        for (int i = 2;; i++) {
            anfact = modexp(anfact, i, n);
            BigUnsigned d = gcd(anfact-1, n);
            if (d > 1 && d < n) return d;
            else if (d == n) break;
        }
    }
}
