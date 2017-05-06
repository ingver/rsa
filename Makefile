CXX = g++
CFLAGS = -Wall

library-objects = \
	bigint/BigUnsigned.o \
	bigint/BigInteger.o \
	bigint/BigIntegerAlgorithms.o \
	bigint/BigUnsignedInABase.o \
	bigint/BigIntegerUtils.o \

RSA: main.cpp library
	$(CXX) $(CFLAGS) -o RSA main.cpp $(library-objects)

#%.o: %.cpp %.h
# 	@$(CXX) $(CFLAGS) -c $<

library:
	make -C bigint library

clean:
	rm -f RSA

clean-all: clean
	make -C bigint clean
# 	@if [ -f *.o ]; then rm *.o; fi
