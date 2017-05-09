#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;

void show_help(const char* prog_name)
{
    cout << "USAGE:\n"
         << prog_name << " <KEY_FILENAME> <INPUT_FILENAME> <OUTPUT_FILENAME>\n\n";
}

/*
 * joins hex ascii codes of each char of the text
 */
string textToHex(const string& text)
{
    unsigned short mask = 0x00ff;
    stringstream s;
    s << setfill('0') << hex;
    for (const char& c: text) {
        unsigned short n = (mask & (unsigned short)c);
        s << setw(2) << n;
    }

    return s.str();
}
/*
 * splits text to get hex ascii codes
 */
string hexToText(const string& text)
{
    stringstream in(text), out;
    string h;
    while (in >> setw(2) >> h) {
        unsigned short i = stoi(h, 0, 16);
        out.put(i);
    }
    return out.str();
}

/*
 * converts text to BigUnsigned
 */
BigUnsigned textToBigUnsigned(const string& text)
{
    return BigUnsignedInABase(textToHex(text), 16);
}

/*
 * converts BigUnsigned to text
 */
string bigUnsignedToText(const BigUnsigned& bu)
{
    stringstream s;
    s << hex << bu;

    string str(s.str());
    if (s.str().length() % 2 != 0) {
        str = string("0") + str;
    }

    return hexToText(str);
}

int main(int argc, const char** argv)
{
    /*
     * testing conversion from text to bigint and vice versa
     */

    if (argc != 4) {
        show_help(argv[0]);
        return 1;
    }

    /*
     * read arguments
     */
    string key_filename(argv[1]),
           input_filename(argv[2]),
           output_filename(argv[3]);

    /*
     * open key file
     */
    fstream key_file(key_filename, fstream::in);
    if (!key_file.is_open()) {
        cerr << "Failed to open KEY file `" << key_filename << "`" << endl;
        return 4;
    }

    /*
     * read the key
     */
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

    /*
     * open input file
     */
    fstream input_file(input_filename, fstream::in);
    if (!input_file.is_open()) {
        cerr << "Failed to open INPUT file `" << input_filename << "`" << endl;
        return 6;
    }

    /*
     * read the message
     */
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

    /*
     * convert input to BigUnsigned
     */
    BigUnsigned input(textToBigUnsigned(input_str));

    /*
     * check if input is greater than key modulus
     */
    if (input > mod) {
        cerr << "The key must be greater than the input!" << endl;
        return 8;
    }

    /*
     * make operation
     */
    BigUnsigned result(modexp(input, exp, mod));
    BigUnsigned output(result);
    string output_str(bigUnsignedToText(output));

    /*
     * check if output is accidentally got greater than mod
     */
    if (output > mod) {
        cerr << "FATAL ERROR: output is greater than mod" << endl;
        return 9;
    }

    /*
     * open output file
     */
    fstream output_file(output_filename, fstream::out);
    if (!output_file.is_open()) {
        cerr << "Failed to open OUTPUT file `" << output_filename << "`" << endl;
        return 10;
    }

    /*
     * write the result
     */
    output_file << output_str;
    if (output_file.fail()) {
        cerr << "An error occured while writing the result to OUTPUT file";
    }

    /*
     * close output file
     */
    output_file.close();

    return 0;
}
