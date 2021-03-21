#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include "rsa.cpp"


int main() {

    rsa encrypt;

    std::cout << "p = " << encrypt.p << std::endl 
              << "q = " << encrypt.q << std::endl 
              << "phi_n = " << encrypt.phi_n << std::endl 
              << "if coprime 1 = " << (encrypt.q * encrypt.p) % encrypt.e << std::endl 
              << "d = " << encrypt.d << std::endl
              << "e = " << encrypt.e << std::endl
              << "if e and d mod inverses 1 = " << encrypt.d * encrypt.e % encrypt.phi_n << std::endl;

    //for(int i = 0; i < 100; i++)
    //    if(encrypt.isPrime(i))
    //        std::cout << i << " ";

    //for(int i = 0; i < 100; i++)
    //    std::cout << encrypt.randPrime(8) << " ";

    //boost::multiprecision::int1024_t i(ULLONG_MAX);
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //i = i * ULLONG_MAX;
    //std::cout << i << std::endl;

    return 0;
}
