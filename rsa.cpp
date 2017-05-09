#include <algorithm>
#include <iostream>
#include <fstream>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;

void show_help(const char* prog_name)
{
    cout << "USAGE:\n"
         << prog_name << " e|d [<KEY_FILENAME>] [<INPUT_FILENAME>] [<OUTPUT_FILENAME>]\n\n"
         << "    e -- encrypt\n"
         << "    d -- decrypt\n\n"
         << "defaults:\n"
         << "    KEY_FILENAME: e -> \"rsa_pub.key\" | d -> \"rsa_priv.key\"\n"
         << "    INPUT_FILENAME: \"input.txt\"\n"
         << "    OUTPUT_FILENAME: \"output.txt\"\n";
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
    string data((const char *)(arr.blk), size_in_bytes);

    //reverse(begin(data), end(data));

    return data;
}

int main(int argc, const char** argv)
{
    //string msg_str = "Hello, RSA";
    //BigUnsigned msg(dataToBigUnsigned(msg_str.c_str(), msg_str.length()));

    if (argc < 2 || argc > 5) {
        show_help(argv[0]);
        return 1;
    }

    string operation(argv[1]);

    if (operation.length() != 1) {
        cerr << "Second parameter corresponds to operation `e` (encrypt) or `d` (decrypt)\n";
        show_help(argv[0]);
        return 2;
    }

    /*
     * crypto operation
     * e -- encrypt
     * d -- decrypt
     */
    char op = operation[0];

    // filename that contains crypto-key
    string key_filename;
    // set default filename
    if (op == 'e') {
        key_filename = "rsa_pub.key"; // default public key filename
    } else if (op == 'd') {
        key_filename = "rsa_priv.key"; // default private key filename
    } else {
        cout << "Wrong operation `" << op << "`. Must be `e` or `d`\n";
        show_help(argv[0]);
        return 3;
    }

    // default filenames for input and output
    string input_filename("input.txt"),
           output_filename("output.txt");

    // read arguments
    if (argc >= 3) {
        key_filename = argv[2];
    }
    if (argc >= 4) {
        input_filename = argv[3];
    }
    if (argc == 5) {
        output_filename = argv[4];
    }


    // open key file
    fstream key_file(key_filename, fstream::in);
    if (!key_file.is_open()) {
        cerr << "Failed to open KEY file `" << key_filename << "`" << endl;
        return 4;
    }

    // read the key
    string exp_str, mod_str;
    getline(key_file, exp_str);
    getline(key_file, mod_str);
    if (key_file.fail()) {
        cerr << "An error occured while reading KEY file!" << endl;
        return 5;
    }
    key_file.close();

    BigUnsigned exp(stringToBigUnsigned(exp_str)),
                mod(stringToBigUnsigned(mod_str));
    cout << "exp = " << exp << endl
         << "mod = " << mod << endl
         << "mod capacity = " << mod.getCapacity() << endl;

    // open input file
    fstream input_file(input_filename, fstream::in);
    if (!input_file.is_open()) {
        cerr << "Failed to open INPUT file `" << input_filename << "`" << endl;
        return 6;
    }

    // read the message
    string input_str, line;
    if (getline(input_file, line)) {
        input_str += line;
    }
    while (getline(input_file, line)) {
        input_str += string("\n") + line;
    }
    if (!input_file.eof() && input_file.fail()) {
        cerr << "An error occured while reading INPUT file!" << endl;
        return 7;
    }
    input_file.close();

    cout << "---input----\n"
         << input_str << endl
         << "------------\n";

    // convert input to BigUnsigned
    BigUnsigned input(dataToBigUnsigned(input_str.c_str(), input_str.length()));
    cout << "input:\n"
         << input << endl
         << "input capacity = " << input.getCapacity() << endl;

    string input_str2(unsignedToData(input));
    cout << "---input2----\n"
         << input_str2 << endl
         << "------------\n";

    cout << "input == input2 : " << (input_str == input_str2) << endl;

    if (input > mod) {
        cerr << "The key must be greater than the input!" << endl;
        return 8;
    }

    // make operation
    BigUnsigned result = modexp(input, exp, mod);
    BigUnsigned output(result);
    string output_str(unsignedToData(output));

    cout << "---output----\n"
         << output_str << endl
         << "------------\n";
    cout << "output:\n"
         << output << endl
         << "output capacity = " << output.getCapacity() << endl;

    if (output > mod) {
        cerr << "FATAL ERROR: output is greater than mod" << endl;
        return 9;
    }

    // open output file
    fstream output_file(output_filename, fstream::out);
    if (!output_file.is_open()) {
        cerr << "Failed to open OUTPUT file `" << output_filename << "`" << endl;
        return 10;
    }

    // write the result
    output_file << output_str;
    if (output_file.fail()) {
        cerr << "An error occured while writing the result to OUTPUT file";
    }

    // close output file
    output_file.close();

    //BigUnsigned enc_msg = modexp(msg, e, n);
    //string enc_msg_str = unsignedToData(enc_msg);

    //BigUnsigned dec_msg = modexp(enc_msg, d, n);
    //string dec_msg_str = unsignedToData(dec_msg);

    return 0;
}
