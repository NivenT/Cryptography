//#define ATTACK

#define ELGAMAL     0
#define USING_RSA   1

#define ENCRYPTION  1 /// Change this value to change the type of encryption

#include <iostream>
#include <chrono>

#include "Elgamal.h"
#include "DLPAttacker.h"
#include "RSA.h"

using namespace std;

int main() {
    #if (ENCRYPTION == ELGAMAL)
        CryptoScheme* crypt = new Elgamal;
    #elif (ENCRYPTION == USING_RSA)
        CryptoScheme* crypt = new RSA;
    #endif // ENCRYPTION

    #ifdef ATTACK
        crypt->init(30); //takes ~50 minutes on my computer to crack, 20 takes ~10 seconds
    #else
        crypt->init(200);
    #endif //ATTACK

    string msg = "Hello, World";
    vector<BigUnsigned> encoding = CryptoScheme::encode(msg);
    vector<void*> cipher(encoding.size());
    for (int i = 0; i < encoding.size(); i++) {
        cipher[i] = crypt->encrypt(encoding[i]);
        encoding[i] = crypt->decrypt(cipher[i]);
    }
    cout<<"Original message:  "<<msg<<endl
        <<"Decrypted message: "<<CryptoScheme::decode(encoding)<<endl
        <<endl;

    #ifdef ATTACK
        cout<<"Attacking encryption..."<<endl;
        auto start = chrono::high_resolution_clock::now();

        vector<BigUnsigned> decryption(cipher.size());
        BigUnsigned mod = crypt->getModulus();
        #if (ENCRYPTION == ELGAMAL)
            Elgamal* eCrypt = (Elgamal*)crypt;

            BigUnsigned order = CryptoScheme::findOrder(eCrypt->g, mod);
            for (int i = 0; i < cipher.size(); i++) {
                Epair p = *(Epair*)cipher[i];
                BigUnsigned k = DLPAttacker::babyGiant(eCrypt->g, p.first, mod, order);
                decryption[i] = (p.second*modinv(modexp(eCrypt->pub, k, mod), mod))%mod;
            }
        #elif (ENCRYPTION == USING_RSA)
        #endif // ENCRYPTION
        chrono::seconds durr = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start);
        cout<<"Decrypted message: "<<CryptoScheme::decode(decryption)<<endl
            <<"Took "<<durr.count()/60<<" minutes and "<<durr.count()%60<<" seconds to break.";
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
