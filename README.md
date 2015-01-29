# Fiat-Shamir_ZK-Protocol
OOP Fiat-Shamir Zero Knowledge Protocol implementation

Library includes:

-Key struct;

-the fun FS_key_create that take 2 uninitialized Key variable and generate public and private key;

-Proover class

-Verifier class

detail in FS_class.cpp file

Random Number Generator

Library not includes a Random Number Generator, user can replace it with one of your own in Define.h file 

There are two generators:

-RandNum: return a big-int number (compatible with arbitrary precision library used)

-Randbit: return a simple int with value 0 or 1

-Randinit and Randbitinit are the respective initializers 

Credit

Arbitrary Precision Library from TTMath (not included in the source) Link: http://www.ttmath.org


:)
