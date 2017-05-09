# RSA implementation in C++

Compile with:
```bash
$ make
```

Generate keys with:
```
./gen_keys <KEY_SIZE_IN_BITS> [<PUB_KEY_FILENAME> [<PRIV_KEY_FILENAME]]

defaults:
PUB_KEY_FILENAME: "rsa_pub.key"
PRIV_KEY_FILENAME: "rsa_prv.key"
```

Cipher/decipher with:
```
./rsa <KEY_FILENAME> <INPUT_FILENAME> <OUTPUT_FILENAME>
```


*For big integers I used [this](https://mattmccutchen.net/bigint/) library*
