#ifndef RSA
#define RSA

#include <boost/multiprecision/cpp_int.hpp>

class rsa {

private:
public:
    boost::multiprecision::uint1024_t p,
             q,
             n,
             phi_n,
             e = 257, // bad without padding but fast
             d;

    boost::multiprecision::uint1024_t randPrime(int bits);
    bool isPrime(boost::multiprecision::uint1024_t n);

    bool calcN();
    bool calcPhi_n();

    bool calcD();

    rsa();

    std::string encrypt(std::string plaintext);
    std::string decrypt(std::string ciphertext);
    boost::multiprecision::uint1024_t getPubKey();
};

#endif
