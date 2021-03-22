#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/multiprecision/cpp_int.hpp>
#include "rsa.cpp"


int main() {

    rsa encrypt,
        decrypt;

    std::cout << "encrypt" << std::endl
              << "p = " << encrypt.p << std::endl
              << "q = " << encrypt.q << std::endl
              << "phi_n = " << encrypt.phi_n << std::endl
              << "d = " << encrypt.d << std::endl
              << "e = " << encrypt.e << std::endl;

    std::cout << "decrypt" << std::endl
              << "p = " << decrypt.p << std::endl
              << "q = " << decrypt.q << std::endl
              << "phi_n = " << decrypt.phi_n << std::endl
              << "d = " << decrypt.d << std::endl
              << "e = " << decrypt.e << std::endl;

    std::ifstream in("plain.txt");
    std::ofstream out("crypt.txt");
    std::stringstream message;
    message << in.rdbuf();

    out << encrypt.encrypt(decrypt.e, decrypt.n, message.str());

    std::cout << message.str()
              << encrypt.encrypt(decrypt.e, decrypt.n, message.str()) << std::endl
              << decrypt.decrypt(message.str());

    return 0;
}
