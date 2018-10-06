CXXFLAGS = -std=c++17 -g -W -Wall -Wextra
Objects = mainTest.o FiatShamirProtocol.o BigInteger.o Test.o
LibFlag = -L/usr/local/lib/ -lgmp -lgmpxx

Test : $(Objects)
	$(CXX) $(Objects) $(LibFlag) -o FSTest

Library :  CXXFLAGS = -std=c++17 -03 $(LibFlag)
Library :  FiatShamirProtocol.o BigInteger.o
	ar rcs FSLib.a $^

