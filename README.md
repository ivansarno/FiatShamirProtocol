# Fiat-Shamir_ZK-Protocol
OOP Fiat-Shamir Zero Knowledge Protocol implementation

Library includes:

-Key struct;

-the fun FS_key_create that take 2 uninitialized Key variable and generate public and private key;

-Proover class

-Verifier class

detail in FS_class.cpp file

Random Number Generator

Random Number Generator included is only for testing, user can replace it with one of your own in Define.h file 

There are two generators:

-RandNum: return a big-int number (compatible with arbitrary precision library used)

-Randbit: return a simple int with value 0 or 1

-Randinit and Randbitinit are the respective initializers 

Usage:

-the client create the keys withFS_key_create

-the client instantiates a Proover object with the private key

-the server instantiates a Verifier object with the public key of client

-run a session of FS exchanging the result of methods for N iteration, with N is the precision you like

Credit

Arbitrary Precision Library from GMP (not included in the source) Link: https://gmplib.org


:)
