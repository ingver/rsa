#include <iostream>
#include "bigint/BigIntegerLibrary.hh"

using namespace std;

int main(int argc, char *argv[])
{
    BigInteger a = 65536;
    cout << (a * a * a) << endl;
    return 0;
}
