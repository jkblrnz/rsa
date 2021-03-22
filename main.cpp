#include <iostream>
#include "rsa.cpp"

int main() {

    rsa encrypt(5, 13);

    //for(int i = 0; i < 100; i++)
    //    if(encrypt.isPrime(i))
    //        std::cout << i << " ";

    /*for(int i = 0; i < 100; i++)
        std::cout << encrypt.randPrime(8) << " ";*/

    std::string message;
    message = "SAMPLETEXTHERE";
    std::cout << message << std::endl;
    std::cout << encrypt.encrypt(message) << std::endl;
    std::cout << encrypt.decrypt(message) << std::endl;
    return 0;
}
