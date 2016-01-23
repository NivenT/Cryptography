#include <iostream>

#include "Elgamal.h"

using namespace std;

int main() {
    CryptoScheme* crypt = new Elgamal;
    BigUnsigned p = CryptoScheme::genPrime(101);
    crypt->init(100, p);


    string msg = "Hello, World";
    vector<BigUnsigned> encoding = CryptoScheme::encode(msg, p);
    for (int i = 0; i < encoding.size(); i++) {
        /*
        cout<<i<<": "<<encoding[i]<<" -> "<<endl;
        Epair temp = *(Epair*)crypt->encrypt(encoding[i]);
        cout<<"    "<<temp.first<<" and "<<temp.second<<endl;
        */
        encoding[i] = crypt->decrypt(crypt->encrypt(encoding[i]));
    }
    cout<<"Original message:  "<<msg<<endl
        <<"Decrypted message: "<<CryptoScheme::decode(encoding, p)<<endl;
    return 0;
}
