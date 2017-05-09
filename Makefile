CXX = g++
CFLAGS = -Wall

library-objects = \
	bigint/BigUnsigned.o \
	bigint/BigInteger.o \
	bigint/BigIntegerAlgorithms.o \
	bigint/BigUnsignedInABase.o \
	bigint/BigIntegerUtils.o \

rsa: rsa.cpp library
	$(CXX) $(CFLAGS) -o rsa rsa.cpp $(library-objects)

gen_keys: gen_keys.cpp library
	$(CXX) $(CFLAGS) -o gen_keys gen_keys.cpp $(library-objects)

all: RSA gen_keys

#%.o: %.cpp %.h
# 	@$(CXX) $(CFLAGS) -c $<

library:
	make -C bigint library

clean:
	rm -f rsa gen_keys **/*.o

clean-all: clean
	make -C bigint clean
# 	@if [ -f *.o ]; then rm *.o; fi
