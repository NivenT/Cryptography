//#define DEBUG

#ifdef DEBUG
    #include <iostream>

    using namespace std;
#endif // DEBUG

#include <unordered_map>

#include "ECDLPAttacker.h"

struct BigUnsignedHasher {
    std::size_t operator()(const BigUnsigned& n) const {
        std::size_t h = 0;
        for (int i = 0; i < n.getLength(); i++) {
            h ^= n.getBlock(i) << i;
        }
        return h;
    }
};

struct ECPointHasher {
    std::size_t operator()(const ECPoint& p) const {
        if (p.isIdeal()) return 0;
        Polynomial x = p.getX().getVal(), y = p.getY().getVal();
        std::size_t h = 0;
        for (int i = 0; i < x.size(); i++) {
            h ^= BigUnsignedHasher{}(x[i].getVal()) << i;
        }
        for (int i = 0; i < y.size(); i++) {
            h ^= BigUnsignedHasher{}(y[i].getVal()) << i;
        }
        return h;
    }
};

BigUnsigned ECDLPAttacker::naive(const ECPoint& p, const ECPoint& q) {
    BigUnsigned n = 0;
    ECPoint r = p.getCurve()->makeElement();
    while (r != q) {
        r = r + p;
        n++;
    }
    return n;
}

BigUnsigned ECDLPAttacker::babyGiant(const ECPoint& p, const ECPoint& q, const BigUnsigned& order) {
    BigUnsigned o = (order == 0) ? p.findOrder() : order;
    BigUnsigned n = (o >> (o.bitLength()/2)); ///approximately sqrt(o)
    #ifdef DEBUG
    cout<<"P:     "<<p<<endl
        <<"Q:     "<<q<<endl
        <<"order: "<<o<<endl
        <<"n:     "<<n<<endl
        <<endl;
    #endif // DEBUG
    ECPoint u = -p*n;
    ECPoint l1 = p.getCurve()->makeElement(), l2 = q;
    std::unordered_map<BigUnsigned, ECPoint, BigUnsignedHasher> list1;
    std::unordered_map<ECPoint, BigUnsigned, ECPointHasher> list2;
    for (BigUnsigned i = 0; i <= n; i++) {
        list1[i] = l1;
        list2[l2] = i;

        l1 = l1+p;
        l2 = l2+u;
    }
    for (BigUnsigned i = 0; i <= n; i++) {
        auto it = list2.find(list1[i]);
        if (it != list2.end()) {
            n *= it->second; n += i;
            #ifdef DEBUG
            cout<<"i:     "<<i<<endl
                <<"j:     "<<it->second<<endl
                <<"i+j*n: "<<n<<endl
                <<"iP:    "<<list1[i]<<endl
                <<"Q+jU:  "<<it->first<<endl
                <<endl;
            #endif // DEBUG
            return n;
        }
    }
}
