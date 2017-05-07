#include <iostream>
#include <fstream>
#include <vector>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;

void show_help(const char* prog_name)
{
    cout << "USAGE:\n"
         << prog_name << " <SIZE_IN_BYTES> [<PUB_KEY_FILENAME>] [<PRIV_KEY_FILENAME]\n\n"
         << "defaults:\n"
         << "PUB_KEY_FILENAME: \"rsa_pub.key\"\n"
         << "PRIV_KEY_FILENAME: \"rsa_priv.key\"\n";
}

/*
 * yeilds vector of:
 * e -- open exponent
 * d -- private exponent
 * n -- modulus
 */
vector<BigUnsigned> generate_keys(size_t max_size)
{
    //BigUnsigned p = 65537;
    //BigUnsigned q = 17;
    //BigUnsigned p = 3557;
    //BigUnsigned q = 2579;
    BigUnsigned p = 2147483647,
                q = 305175781,
                n = p * q,
                fi_n = (p - 1) * (q - 1),
                e = 65537,
                d = modinv(e, fi_n);

    //cout << "p = " << p << endl
         //<< "q = " << q << endl
         //<< "n = p * q = " << n << endl
         //<< "fi(n) = " << fi_n << endl
         //<< "e = " << e << endl
         //<< "d = " << d << endl
         //<< "e * d = " << e * d << endl
         //<< "e * d % fi_n = " << e * d % fi_n << endl;

    return vector<BigUnsigned>{
        e, d, n
    };
}

int main(int argc, const char** argv)
{
    size_t max_size = 1024;
    const char* pub_filename = "rsa_pub.key",
              * priv_filename = "rsa_priv.key";

    if (argc < 2 || argc > 4) {
        show_help(argv[0]);
        return 1;
    }

    max_size = stoi(string(argv[1]));

    if (argc > 2) {
        pub_filename = argv[2];
    }
    if (argc > 3) {
        priv_filename = argv[3];
    }

    vector<BigUnsigned> keys(generate_keys(max_size));

    BigUnsigned e = keys[0], d = keys[1], n = keys[2];

    fstream pub_file(pub_filename, fstream::out);
    pub_file << e << endl << n << endl;
    pub_file.close();

    fstream priv_file(priv_filename, fstream::out);
    priv_file << d << endl << n << endl;
    priv_file.close();

    cout << "\nGenerated keys:\n";
    cout << "{e, n} = {" << e << ", " << n << "}\n";
    cout << "{d, n} = {" << d << ", " << n << "}\n";
    //cout << "to hex:\n";
    //cout << "{e, n} = {" << showbase << hex << e << ", " << n << "}\n";
    //cout << "{d, n} = {" << hex << d << ", " << n << "}\n\n";

    return 0;
}
