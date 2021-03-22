#ifndef RSA
#define RSA

#include <boost/multiprecision/cpp_int.hpp>

namespace mp = boost::multiprecision;

class rsa {
private:
    bool calcPhi_n();
    bool calcD();
    bool calcN();

public:
    mp::uint1024_t p, // not private for demonstration purposes
                   q,
                   phi_n,
                   d,
                   n,
                   e = 257;

    rsa();
    rsa(int bits);

    std::string encrypt(mp::uint1024_t other_e, mp::uint1024_t other_n, std::string plaintext);
    std::string decrypt(std::string ciphertext);
};

#endif
