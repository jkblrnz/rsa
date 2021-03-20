#ifndef RSA
#define RSA

class rsa {

private:
public:
    unsigned p,
             q,
             n,
             phi_n,
             e = 3, // bad without padding but fast
             d;

    unsigned randPrime(int bits);
    bool isPrime(unsigned n);

    bool calcN();
    bool calcPhi_n();

    bool calcD();

    rsa();
    rsa(unsigned p, unsigned q);

    unsigned getPubKey();
};

#endif
