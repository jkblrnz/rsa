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

unsigned char_to_num(char a)
{
    int x;
    x = a - 65; //Messy conversion: A is 65 in ASCII, so this makes it "0" in our code.
    //A = 0, B = 1, C = 2, etc.
    //a = 32, b = 33, etc.
    //Convert lowercase to uppercase if necessary
    if(x >= 32)
        x -= 32;
    //This will handle characters, but punctuation could cause issues, so pretty please don't do that
    return x;
}

char num_to_char(unsigned a)
{
    char x;
    x = a + 65;
    //This will handle characters, but punctuation could cause issues, so pretty please don't do that
    return x;
}


//When given a plaintext string as input, convert to a ciphertext and output
//This function does NOT read/write files
std::string rsa::encrypt(std::string plaintext)
{
    std::string ciphertext = ""; //Store this for later
    unsigned c = 0;
    unsigned nums[plaintext.length()];
    unsigned temp;
    unsigned trigraphs[plaintext.length()]; //A little bigger than it needs to be, but will do the job
    unsigned tri_count = 0; //Will keep track of used spaces in the above array
    //Fill whitespace on the end if necessary until the total space is a multiple of 3
    unsigned quadgraphs[plaintext.length()]; //Bigger than it needs to be
    unsigned quad_count = 0;
    unsigned q1, q2, q3, q4;

    //Add "whitespace" as needed
    while(plaintext.length() % 3 != 0)
    {
        plaintext.append("A");
    }
    //Iterate through the string and encode it into numbers
    while(c < plaintext.length())
    {
        //Convert each character to a number and put it in the array
        nums[c++] = char_to_num(plaintext[c]);
    }
    //Convert every three characters into a single trigraph number
    c = 0;//Reset c
    while(tri_count < plaintext.length() / 3)
    {
        temp = 0;
        temp += nums[c++] * 26 * 26;
        temp += nums[c++] * 26;
        temp += nums[c++];
        trigraphs[tri_count++] = temp; //It's not pretty, but this encodes three numbers at a time as a trigraph
        //and iterates through the plaintext and trigraph at the same time (but at different rates)
    }
    //Iterate through the trigraphs and encrypt each of them, then turn to completed ciphertext and stored in the quadgraphs array
    for(int d = 0; d < tri_count; d++)
    {
        //The fabled encrypt function, p^e % n
        temp = static_cast<unsigned>(power(trigraphs[d],e, n));
        //Calculate the four quotients
        q1 = 0;
        while(temp >= 26 * 26 * 26)
        {
            temp -= 26 * 26 * 26;
            q1++;
        }
        q2 = 0;
        while(temp >= 26 * 26)
        {
            temp -= 26 * 26;
            q2++;
        }
        q3 = 0;
        while(temp >= 26)
        {
            temp -= 26;
            q3++;
        }
        q4 = temp;
        //Tally these to create a ciphertext, and place it into the quadgraph array
        quadgraphs[quad_count++] = q1;
        quadgraphs[quad_count++] = q2;
        quadgraphs[quad_count++] = q3;
        quadgraphs[quad_count++] = q4;
    }
    //Lastly, encode the info as a ciphertext
    for(int d = 0; d < quad_count; d++)
    {
        std::string t = " ";
        t[0] = num_to_char(quadgraphs[d]);
        ciphertext.append(t);
    }
    return ciphertext;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//When given a ciphertext string as input, convert to plaintext and output
//This function does NOT read/write files
std::string rsa::decrypt(std::string ciphertext)
{
    unsigned temp;
    unsigned quadgraph[ciphertext.length()];
    unsigned trigraph[ciphertext.length()]; //Bigger than we need, but it'll work
    unsigned tri_length = 0;
    int c = 0;
    //Start by turning the inputted ciphertext into numerical quadgraphs
    while(c < ciphertext.length())
    {
        quadgraph[c++] = char_to_num(ciphertext[c]);
    }
    //Turn every four quadgraphs into one trigraph
    c = 0;
    while(c < ciphertext.length())
    {
        temp = 0;
        temp += (quadgraph[c] * 26 * 26 * 26);
        c++;
        temp += (quadgraph[c] * 26 * 26);
        c++;
        temp += (quadgraph[c] * 26);
        c++;
        temp += (quadgraph[c]);
        c++;
        trigraph[tri_length++] = temp;
    }
    //Decrypt each trigraph
    c = 0;
    while(c < tri_length)
    {
        temp = trigraph[c++];
        temp = static_cast<unsigned>(power(temp,d, n));
    }
    return ciphertext;
}
