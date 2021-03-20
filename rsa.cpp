#include <random>
#include <time.h>
#include "rsa.h"

rsa::rsa(unsigned a, unsigned b) {
    std::srand(time(0));
    p = a;
    q = b;
    calcN();
    calcPhi_n(); // phi_n has to be found before D can be
    calcD();
}

// (a ^ exp) mod n
unsigned power(unsigned a, unsigned exp, unsigned n) {
    unsigned res = 1;
    a = a % n;

    while (exp > 0) {
        if (exp & 1)
            res = (res * a) % n;
        exp /= 2;
        a = (a * a) % n;
    }

    return res;
}

bool millerRabin(unsigned d, unsigned n) { // edit to clean up
    int a = 2 + std::rand() % (n - 4); // this step can reduce the number of req iterations

    int x = power(a, d, n);

    if (x == 1 || x == n - 1) return true;

    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1) return false;
        if (x == n - 1) return true;
    }

    return false;
}

bool rsa::isPrime(unsigned n) {
    int k = 40; // repeat miller robin to reduce chance of error .75 ^ k = err 40 is the limit to efectiveness

    //millerRabin do not handle less than 5 well so this checks for edge cases
    if (n <= 1 || n == 4) return false; // 4, 0, and negative numbers are not prime
    if (n <= 3) return true; // 2 and 3 are prime

    // figure out better
    int d = n - 1;
    while (d % 2 == 0) // find the first one with an odd power
        d /= 2; // is this the witness?

    //loop millerRabin if one test comes false the number wasn't prime after all
    for(int i = 0; i < k; i++)
        if(!millerRabin(d, n))
            return false;

    return true;
}

unsigned rsa::randPrime(int bits) { // kinda bad to have a potentially forever loop, but there should be no cases where it does go forever
    unsigned max = bits != 8 ? (1 << bits) - 1 : 511, // special case when at max size of var to prevent overflow will need to be smarter for a 512 bit
             min = 1 << (bits - 1),
             genPrime = 0;

    max >>= 1;
    min >>= 1;

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

unsigned xGcd(unsigned a, unsigned b, unsigned &x, unsigned &y) { // x and y are the mult parts not entirely useful for us, but ill include it
    if(b == 0) { // base case
        x = 1;
        y = 0;
        return a;
    }

    unsigned x1, y1,
             gcd = xGcd(b, a % b, x1, y1); // recursion might wanna do iterative but for now good enough
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

// e is predetermined from the acceptable list find D from e's modular inverse
// calcPhi_n should be ran before this function
bool rsa::calcD() {
    unsigned x, y; // can be useful for debug
    d = xGcd(e, phi_n, x, y);
    return true;
}

unsigned rsa::getPubKey() {
    return 0;
}
