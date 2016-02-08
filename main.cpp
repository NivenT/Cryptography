#define TEST_FIELDS
#define ATTACK

#define ELGAMAL     0
#define USING_RSA   1

#define ENCRYPTION  1 /// Change this value to change the type of encryption

#include <iostream>
#include <chrono>

#include "Elgamal.h"
#include "DLPAttacker.h"
#include "RSA.h"
#include "IntegerFactorer.h"
#include "FiniteField.h"

using namespace std;

int main() {
    CryptoScheme::seedRNG();
    #ifdef TEST_FIELDS
        PrimeField f23(23);
        FieldElement a = f23.makeElement(11), b = f23(20);

        PolyDomain p23(&f23);
        Polynomial c1 = p23({f23(12),f23(23),f23(33),f23(46),f23(15)}),
                   c2 = p23({f23(64),f23(57),f23(84)});

        FiniteField f(23, 3);
        FFElement   d1 = f(c1),
                    d2 = f(c2);

        cout<<a<<" + "<<b<<" = "<<a+b<<endl
            <<a<<" - "<<b<<" = "<<a-b<<endl
            <<a<<" * "<<b<<" = "<<a*b<<endl
            <<a<<" / "<<b<<" = "<<a/b<<endl
            <<"-"<<a<<" = "<<-a<<endl
            <<"("<<a<<")^-1 = "<<a.inverse()<<endl
            <<endl;
        cout<<"("<<c1<<") + ("<<c2<<") = "<<c1+c2<<endl
            <<"("<<c1<<") - ("<<c2<<") = "<<c1-c2<<endl
            <<"("<<c2<<") - ("<<c1<<") = "<<c2-c1<<endl
            <<"-("<<c1<<") = "<<-c1<<endl
            <<a<<" * ("<<c1<<") = "<<c1*a<<endl
            <<"("<<c1<<") * ("<<c2<<") = "<<c1*c2<<endl
            <<c1<<" = ("<<c1/c2<<")*("<<c2<<") + ("<<c1%c2<<")"<<endl
            //<<"("<<c1<<") % ("<<c2<<") = "<<c1%c2<<endl
            <<endl;
        cout<<"random polynomial of degree 5: "<<p23.randomPoly(5)<<endl
            <<"irreducible polynomial of degree 5: "<<p23.generateIrreducible(5)<<endl
            <<endl;
        cout<<"The modulus is "<<f.getMod()<<endl
            <<"("<<d1<<") + ("<<d2<<") = "<<d1+d2<<endl
            <<"("<<d1<<") - ("<<d2<<") = "<<d1-d2<<endl
            <<"("<<d1<<") * ("<<d2<<") = "<<d1*d2<<endl
            <<"-("<<d1<<") = "<<-d1<<endl
            <<"("<<d1<<")^-1 = "<<d1.inverse()<<endl
            <<"("<<d1.inverse()<<") * ("<<d1<<") = "<<d1.inverse()*d1<<endl
            <<"("<<d1<<") / ("<<d2<<") = "<<d1/d2<<endl
            <<"("<<d1/d2<<") * ("<<d2<<") = "<<(d1/d2)*d2<<endl;
    #else
        #if (ENCRYPTION == ELGAMAL)
            CryptoScheme* crypt = new Elgamal;
        #elif (ENCRYPTION == USING_RSA)
            CryptoScheme* crypt = new RSA;
        #endif // ENCRYPTION

        #ifdef ATTACK
            #if (ENCRYPTION == ELGAMAL)
                crypt->init(25);
            #elif (ENCRYPTION == USING_RSA)
                crypt->init(50);
            #endif //ENCRYPTION
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
                RSA* rCrypt = (RSA*)crypt;

                BigUnsigned p = IntFactorer::pollard(rCrypt->getModulus());
                BigUnsigned q = rCrypt->getModulus()/p;
                BigUnsigned d = modinv(rCrypt->pub, (p-1)*(q-1));
                for (int i = 0; i < cipher.size(); i++) {
                    BigUnsigned c = *(BigUnsigned*)cipher[i];
                    decryption[i] = modexp(c, d, rCrypt->getModulus());
                }
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
    #endif // TEST_FIELDS
    return 0;
}
