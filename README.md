# Fiat-Shamir_ZK-Protocol
OOP Fiat-Shamir Zero Knowledge Protocol implementation

2 version are avaiables, C++ and C#

Library includes:

-the fun KeyGen for keys generation(C++)

-KeyGen class(C#)

-Proover class

-Verifier class

-Support for multithreading

Random Number Generator

Random Number Generator included in C++ version is only for testing, user can replace it subclassing ZKFS::Aux::Generator

The C# version using .NET RandomNumberGenerator abstract class.


Usage:

-the client create the keys with KeyGen

-the client instantiates a Proover object with the private key

-the server instantiates a Verifier object with the public key of client

-run a session of FS exchanging the result of methods for N iteration, with N is the precision you like:

  -Client: Proover.step1 -> Server
  
  -Server: Verifier.step1 -> Client
  
  -Client: Proover.step2 -> Server
  
  -Server: Verifier.step2 -> result of identification
  
  -in alternative (Server: Verifier.checkstate -> result of identification)

Credit

Arbitrary Precision Library for C++ version from GMP (not included in the source) Link: https://gmplib.org


:)
