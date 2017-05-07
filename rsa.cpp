#include <algorithm>
#include <iostream>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;

void show_help(const char* prog_name)
{
    cout << "USAGE:\n"
         << prog_name << " e|d [<KEY_FILENAME>] [<INPUT_FILENAME>] [<OUTPUT_FILENAME>]\n\n"
         << "e -- encrypt\n"
         << "d -- decrypt\n\n"
         << "defaults:\n"
         << "KEY_FILENAME: e -> \"rsa_pub.key\" | d -> \"rsa_priv.key\"\n"
         << "INPUT_FILENAME: \"input.txt\"\n"
         << "OUTPUT_FILENAME: \"output.txt\"\n";
}

BigUnsigned dataToBigUnsigned(const char* data, size_t length)
{
    return dataToBigInteger(data, length, BigInteger::positive).getMagnitude();
}

string unsignedToData(const BigUnsigned& num)
{
    BigUnsigned::Index cap = num.getCapacity();
    NumberlikeArray<BigUnsigned::Blk> arr(cap);

    for (BigUnsigned::Index i = 0; i < cap; ++i) {
        arr.blk[i] = num.getBlock(i);
    }

    size_t size_in_bytes = sizeof(BigUnsigned::Blk) * cap;
    string msg((const char *)(arr.blk), size_in_bytes);

    reverse(begin(msg), end(msg));

    return msg;
}

int main(int argc, const char** argv)
{
    string msg_str = "Hello, RSA";
    BigUnsigned msg(dataToBigUnsigned(msg_str.c_str(), msg_str.length()));

    if (argc < 3 || argc > 5) {
        show_help(argv[0]);
        return 1;
    }

    string operation(argv[1]);

    if (operation.length() != 1) {
        cerr << "Not enough parameters\n";
        show_help(argv[0]);
        return 2;
    }

    /* crypto operation
     * e -- encrypt
     * d -- decrypt
     */
    char op = operation[0];

    /* filename that contains crypto-key */
    string key_filename;
    /* set default filename */
    if (op == 'e') {
        key_filename = "rsa_pub.key"; // default public key filename
    } else if (op == 'd') {
        key_filename = "rsa_priv.key"; // default private key filename
    } else {
        cout << "Wrong operation\n";
        show_help(argv[0]);
        return 3;
    }

    string input_filename("input.txt"),
           output_filename("output.txt");

    //BigUnsigned enc_msg = modexp(msg, e, n);
    //string enc_msg_str = unsignedToData(enc_msg);

    //BigUnsigned dec_msg = modexp(enc_msg, d, n);
    //string dec_msg_str = unsignedToData(dec_msg);

    //cout << "\nmessage:\n";
    ////cout << "str: " << msg_str << endl;
    //cout << "to dec: " << dec << msg << endl;
    //cout << "to hex: " << hex << msg << endl;
    //cout << "\nEncrypted message:\n";
    ////cout << "str: " << enc_msg_str << endl;
    //cout << "to dec: " << dec << enc_msg << endl;
    //cout << "to hex: " << hex << enc_msg << endl;
    //cout << "\nDecrypted message:\n";
    ////cout << "str: " << dec_msg_str << endl;
    //cout << "to dec: " << dec << dec_msg << endl;
    //cout << "to hex: " << hex << dec_msg << endl;

    return 0;
}
