#include <boost/multiprecision/cpp_int.hpp>
#include <random>
#include <time.h>
#include "rsa.h"

namespace mp = boost::multiprecision;

rsa::rsa() {
    std::srand(time(0));

    d = static_cast<mp::uint1024_t>(1);
    while(d == 1) { // ensure theres a valid d for the set of values
        mp::uint1024_t a = 0,
                      b = 0;

        while((a * b) % e != 1) { // we initally pick e which is common so we have to make sure p and q work with e
            a = randPrime(512);
            b = randPrime(512);
            p = a;
            q = b;
        }

        calcN();
        calcPhi_n(); // phi_n has to be found before D can be
        calcD();
    }
}

// (a ^ exp) mod n
mp::uint1024_t power(mp::uint1024_t a, mp::uint1024_t exp, mp::uint1024_t n) {
    mp::uint1024_t res = 1;
    a = a % n;

    while (exp > 0) {
        if (exp & 1)
            res = (res * a) % n;
        exp /= 2;
        a = (a * a) % n;
    }

    return res;
}

bool millerRabin(mp::uint1024_t d, mp::uint1024_t n) { // edit to clean up
    mp::uint1024_t a = 2 + std::rand() % (n - 4); // this step can reduce the number of req iterations

    mp::uint1024_t x = power(a, d, n);

    if (x == 1 || x == n - 1) return true;

    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1) return false;
        if (x == n - 1) return true;
    }

    return false;
}

bool rsa::isPrime(mp::uint1024_t n) {
    int k = 40; // repeat miller robin to reduce chance of error .75 ^ k = err 40 is the limit to efectiveness

    //millerRabin do not handle less than 5 well so this checks for edge cases
    if (n <= 1 || n == 4) return false; // 4, 0, and negative numbers are not prime
    if (n <= 3) return true; // 2 and 3 are prime

    // figure out better
    mp::uint1024_t d = n - 1;
    while (d % 2 == 0) // find the first one with an odd power
        d /= 2; // is this the witness?

    //loop millerRabin if one test comes false the number wasn't prime after all
    for(int i = 0; i < k; i++)
        if(!millerRabin(d, n))
            return false;

    return true;
}

mp::uint1024_t rsa::randPrime(int bits) { // kinda bad to have a potentially forever loop, but there should be no cases where it does go forever
    mp::uint1024_t max = mp::pow(mp::uint1024_t(2),bits), // special case when at max size of var to prevent overflow will need to be smarter for a 512 bit
                  min = mp::pow(mp::uint1024_t(2),bits - 1) - 1,
                  genPrime = 0;

    max /= 2;
    min /= 2;

    while(1) {
        genPrime = 2 * (std::rand() % ((max - 1) - min + 1) + min) + 1; // genPrime is rand odd n bits long
        if(isPrime(genPrime)) // only return when we know its prime
            return genPrime;
    }
}

bool rsa::calcN() {
    n = p * q;
    return true;
}

bool rsa::calcPhi_n() {
    phi_n = (p - 1) * (q - 1);
    return true;
}

mp::uint1024_t xGcd(mp::uint1024_t a, mp::uint1024_t b, mp::int1024_t &x, mp::int1024_t &y) { // x and y are the mult parts not entirely useful for us, but ill include it
    if(a == 0) { // base case
        x = 0;
        y = 1;
        return b;
    }

    mp::int1024_t x1, y1;

    mp::uint1024_t gcd = xGcd(b % a, a, x1, y1); // recursion might wanna do iterative but for now good enough
    x = y1 - x1 * (b / a);
    y = x1;
    return gcd;
}

// e is predetermined from the acceptable list find D from e's modular inverse
// calcPhi_n should be ran before this function
bool rsa::calcD() {
    mp::int1024_t x, y; // can be useful for debug
    mp::uint1024_t key;

    xGcd(e, phi_n, x, y);
    d = static_cast<mp::uint1024_t>((x % phi_n + phi_n) % phi_n);
    return true;
}

mp::uint1024_t rsa::getPubKey() {
    return 0;
}
