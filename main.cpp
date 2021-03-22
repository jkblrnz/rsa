#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include "rsa.cpp"


int main() {

    rsa encrypt;

    std::cout << "p = " << encrypt.p << std::endl 
              << "q = " << encrypt.q << std::endl 
              << "phi_n = " << encrypt.phi_n << std::endl 
              << "d = " << encrypt.d << std::endl
              << "e = " << encrypt.e << std::endl;

    std::string message;
    message = "SAMPLETEXTHERE";
    std::cout << message << std::endl;
    std::cout << encrypt.encrypt(message) << std::endl;
    std::cout << encrypt.decrypt(message) << std::endl;
    return 0;
}
