#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;
typedef BigUnsigned::Blk Blk;
typedef BigUnsigned::Index Index;


void show_help(const char* prog_name)
{
    cout << "USAGE:\n"
         << prog_name << " <KEY_SIZE_IN_BITS> [<PUB_KEY_FILENAME>] [<PRIV_KEY_FILENAME]\n\n"
         << "defaults:\n"
         << "PUB_KEY_FILENAME: \"rsa_pub.key\"\n"
         << "PRIV_KEY_FILENAME: \"rsa_priv.key\"\n";
}

/*
 * generates random unsigned long
 */
unsigned long lrand()
{
    if (sizeof(int) < sizeof(long))
        return (static_cast<long>(rand()) << (sizeof(int) * 8)) |
               rand();

    return rand();
}

/*
 * generates random BigUnsigned
 */
BigUnsigned burand(size_t max_blk_count)
{
    Blk* blks = new Blk[max_blk_count];
    for (Index i = 0; i < max_blk_count; ++i) {
        blks[i] = lrand();
    }
    BigUnsigned a(blks, max_blk_count);
    delete blks;

    return a;
}


/*
 * tests `n` for primality (Fermat's test)
 */
#define TESTS 100
bool is_prime(const BigUnsigned& n)
{
    for(Index i = 0; i < TESTS; i++) {
        // create number `a` for the test
        BigUnsigned a( (burand(n.getCapacity()) % (n - 2)) + 2 );

        /*
        * test `n` against `a`
        */
        if (gcd(a, n) != 1) {
            return false;
        }
        if(modexp(a, n - 1, n) != 1) {
            return false;
        }
    }
    return true;
}

/*
 * generates a prime number of `size` bits in place
 */
BigUnsigned generate_prime(size_t size)
{
    size_t blk_size = sizeof(Blk) * 8,                              // size of one block of BigUnsigned
           blk_count = size / blk_size + (size % blk_size ? 1 : 0); // blocks needed to contain primes
    Blk mask = (~Blk(0)) >> (blk_count * blk_size - size);          // bitmask for discarding greater bits

    //cout << "mask = " << showbase << hex << mask << endl
         //<< "blk_size in bits = " << blk_size << endl
         //<< "prime size in bits = " << size << endl
         //<< "prime in bytes = " << (size / 8 + (size % 8 ? 1 : 0)) << endl
         //<< "prime in blocks = " << blk_count << endl;

    // block order in BigUnsigned is inverted
    Index first_blk = blk_count - 1,
          last_blk = 0;

    // create an array of data for generating prime number
    Blk* blks = new Blk[blk_count];
    for (Index i = 0; i < blk_count; ++i) {
        blks[i] = lrand();
    }
    while (blks[first_blk] == 0) {
        blks[first_blk] = lrand();
    }
    // ensure that the last block is odd
    while (blks[last_blk] % 2 == 0) {
        blks[last_blk] = lrand();
    }
    // discard greater bits
    blks[first_blk] &= mask;

    BigUnsigned num(blks, blk_count);
    delete blks;

    while (!is_prime(num)) {
        num += 2;
    }

    //cout << "Generated prime:\n"
         //<< num << endl
         //<< showbase << hex << num << dec << endl;

    return num;
}

/*
 * yeilds vector of:
 * e -- open exponent
 * d -- private exponent
 * n -- modulus
 */
vector<BigUnsigned> generate_keys(size_t key_size)
{
    size_t half_key_size = key_size / 2;

    BigUnsigned p, q, n, fi_n, e, d;
    // generate primes (p and q)
    p = generate_prime(half_key_size);
    q = generate_prime(half_key_size);
    while (p == q) {
        q = generate_prime(half_key_size);
    }

    // compute modulus (n)
    n = p * q;

    // compute fi_n
    fi_n = (p - 1) * (q - 1);

    // compute public exponent (e)
    e = 65537;

    // compute private exponent (d)
    d = modinv(e, fi_n);

    return vector<BigUnsigned>{
        e, d, n
    };
}

int main(int argc, const char** argv)
{
    srand(time(NULL));

    //BigUnsigned tested(stringToBigUnsigned(argv[1]));
    //cout << "is " << tested << " prime? " << is_prime(tested) << endl;
    //return 0;

    size_t key_size; // generated key size
    const char* pub_filename = "rsa_pub.key", // default public key filename
              * priv_filename = "rsa_priv.key"; // default private key filename

    if (argc < 2 || argc > 4) {
        show_help(argv[0]);
        return 1;
    }

    key_size = stoi(string(argv[1])); // get key size from 2nd parameter[

    if (key_size % 2 != 0) {
        cerr << "Key size must be even" << endl;
        return 2;
    }

    if (argc >= 3) {
        pub_filename = argv[2]; // get public key filename from 3rd parameter
    }
    if (argc == 4) {
        priv_filename = argv[3]; // get private key filename from 4th parameter
    }

    // generate keys
    vector<BigUnsigned> keys;
    try {
        keys = generate_keys(key_size);
    } catch (char const* e) {
        cerr << e << endl;
        return 3;
    }

    // name keys respectively
    BigUnsigned e = keys[0], d = keys[1], n = keys[2];

    // write public key to file
    fstream pub_file(pub_filename, fstream::out);
    pub_file << e << endl << n << endl;
    pub_file.close();

    // write private key to file
    fstream priv_file(priv_filename, fstream::out);
    priv_file << d << endl << n << endl;
    priv_file.close();

    cout << "\nGenerated keys:\n";
    cout << "{e, n} = {" << e << ", " << n << "}\n";
    cout << "{d, n} = {" << d << ", " << n << "}\n";
    cout << "in hex:\n";
    cout << "{e, n} = {" << showbase << hex << e << ", " << n << "}\n";
    cout << "{d, n} = {" << hex << d << ", " << n << "}\n\n";

    return 0;
}
