//#define DEBUG

#ifdef DEBUG
    #include <iostream>

    using namespace std;
#endif // DEBUG

#include <map>

#include "CryptoScheme.h"
#include "DLPAttacker.h"

BigUnsigned DLPAttacker::naive(const BigUnsigned& g, const BigUnsigned& h, const BigUnsigned& p) {
    BigUnsigned gx = 1;
    for (BigUnsigned x = 0;; x++) {
        if (gx == h%p) return x;
        gx = (gx*g)%p;
    }
}

BigUnsigned DLPAttacker::babyGiant(const BigUnsigned& g, const BigUnsigned& h, const BigUnsigned& p, const BigUnsigned& order) {
    BigUnsigned o = (order == 0) ? CryptoScheme::findOrder(g, p) : order;
    BigUnsigned n = (o >> (o.bitLength()/2)); ///approximately sqrt(o)
    //while ((n-1)*(n-1) > o) n--; /// n = 1+floor(sqrt(o))
    #ifdef DEBUG
    cout<<"g:     "<<g<<endl
        <<"h:     "<<h<<endl
        <<"p:     "<<p<<endl
        <<"order: "<<o<<endl
        <<"n:     "<<n<<endl;
    #endif // DEBUG
    BigUnsigned u = modinv(modexp(g, n, p), p);
    BigUnsigned l1 = 1, l2 = h;
    std::map<BigUnsigned, BigUnsigned> list1, list2;
    for (BigUnsigned i = 0; i <= n; i++) {
        list1[i] = l1;
        list2[l2] = i;

        l1 = (l1*g)%p;
        l2 = (l2*u)%p;
    }
    for (BigUnsigned i = 0; i <= n; i++) {
        auto it = list2.find(list1[i]);
        if (it != list2.end()) {
            n *= it->second; n += i;
            #ifdef DEBUG
            cout<<"i:     "<<i<<endl
                <<"j:     "<<it->second<<endl
                <<"i+j*n: "<<n<<endl
                <<"g^i:   "<<list1[i]<<endl
                <<"h*u^j: "<<it->first<<endl
                <<endl;
            #endif // DEBUG
            list1.clear();
            list2.clear();
            return n;
        }
    }
}
