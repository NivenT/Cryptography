//#define ATTACK

#include <iostream>
#include <chrono>

#include "Elgamal.h"
#include "DLPAttacker.h"

using namespace std;

int main() {
    Elgamal* crypt = new Elgamal;
    #ifdef ATTACK
        BigUnsigned p = CryptoScheme::genPrime(31);
        crypt->init(p);
    #else
        BigUnsigned p = CryptoScheme::genPrime(101);
        crypt->init(p);
    #endif //ATTACK

    string msg = "Hello, World";
    vector<BigUnsigned> encoding = CryptoScheme::encode(msg, p);
    vector<void*> cipher(encoding.size());
    for (int i = 0; i < encoding.size(); i++) {
        cipher[i] = crypt->encrypt(encoding[i]);
        encoding[i] = crypt->decrypt(cipher[i]);
    }
    cout<<"Original message:  "<<msg<<endl
        <<"Decrypted message: "<<CryptoScheme::decode(encoding, p)<<endl
        <<endl;

    #ifdef ATTACK
        cout<<"Attacking encryption..."<<endl;
        auto start = chrono::high_resolution_clock::now();

        vector<BigUnsigned> decryption(cipher.size());
        BigUnsigned order = CryptoScheme::findOrder(crypt->g, p);
        for (int i = 0; i < cipher.size(); i++) {
            Epair p = *(Epair*)cipher[i];
            BigUnsigned k = DLPAttacker::babyGiant(crypt->g, p.first, crypt->p, order);
            decryption[i] = (p.second*modinv(modexp(crypt->pub, k, crypt->p), crypt->p))%crypt->p;
        }
        chrono::seconds durr = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start);
        cout<<"Decrypted message: "<<CryptoScheme::decode(decryption, p)<<endl
            <<"Took "<<durr.count()/60<<" minutes and "<<durr.count()%60<<" seconds to break a "<<crypt->keyLength<<" bit key";
        decryption.clear();
    #endif // ATTACK

    delete crypt;
    for (auto& c : cipher) {
        delete c;
    }
    cipher.clear();
    encoding.clear();

    return 0;
}
