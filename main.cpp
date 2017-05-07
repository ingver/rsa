#include <algorithm>
#include <iostream>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;

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

int main(int argc, char *argv[])
{
    //BigUnsigned p = 65537;
    //BigUnsigned q = 17;
    //BigUnsigned p = 3557;
    //BigUnsigned q = 2579;
    BigUnsigned p = 2147483647;
    BigUnsigned q = 305175781;
    BigUnsigned n = p * q;
    BigUnsigned fi_n = (p - 1) * (q - 1);
    //BigUnsigned e = 257;
    BigUnsigned e = 65537;
    BigUnsigned d = modinv(e, fi_n);

    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    cout << "n = p * q = " << n << endl;
    cout << "fi(n) = " << fi_n << endl;
    cout << "e = " << e << endl;
    cout << "d = " << d << endl;
    cout << "e * d = " << e * d << endl;
    cout << "e * d % fi_n = " << e * d % fi_n << endl;

    cout << "\nGenerated keys:\n";
    cout << "{e, n} = {" << e << ", " << n << "}\n";
    cout << "{d, n} = {" << d << ", " << n << "}\n";
    cout << "to hex:\n";
    cout << "{e, n} = {" << showbase << hex << e << ", " << n << "}\n";
    cout << "{d, n} = {" << hex << d << ", " << n << "}\n\n";

    //string msg_str = "Hello, RSA";
    //BigUnsigned msg(dataToBigUnsigned(msg_str.c_str(), msg_str.length()));
    BigUnsigned msg(9199999);
    string msg_str(unsignedToData(msg));

    BigUnsigned enc_msg = modexp(msg, e, n);
    string enc_msg_str = unsignedToData(enc_msg);

    BigUnsigned dec_msg = modexp(enc_msg, d, n);
    string dec_msg_str = unsignedToData(dec_msg);

    cout << "\nmessage:\n";
    //cout << "str: " << msg_str << endl;
    cout << "to dec: " << dec << msg << endl;
    cout << "to hex: " << hex << msg << endl;
    cout << "\nEncrypted message:\n";
    //cout << "str: " << enc_msg_str << endl;
    cout << "to dec: " << dec << enc_msg << endl;
    cout << "to hex: " << hex << enc_msg << endl;
    cout << "\nDecrypted message:\n";
    //cout << "str: " << dec_msg_str << endl;
    cout << "to dec: " << dec << dec_msg << endl;
    cout << "to hex: " << hex << dec_msg << endl;

    return 0;
}
