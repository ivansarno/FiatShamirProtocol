# FiatShamirProtocol
C++ OOP Fiat-Shamir Zero Knowledge Protocol implementation

Library includes:

	-KeyGen function
	-ParallelKeyGen function
	-Proover class
	-Verifier class

**Random Number Generator**

Random Number Generator included is only for testing, user can replace it subclassing FiatShamirProtocol::Utils::Generator 

**Usage:**

	a)the client create the keys with KeyGen or ParallelKeyGen
	b)the client instantiates a Proover object with the private key
	c)the server instantiates a Verifier object with the public key of client
	d)run a session of the protocol exchanging the result of methods for N iteration, with N is the precision you like:

      1)Client: Proover.step1 -> Server
      2)Server: Verifier.step1 -> Client
      3)Client: Proover.step2 -> Server
      4)Server: Verifier.step2 -> result of identification
      4.1)Server: Verifier.checkstate -> result of identification
    
    
**Dependencies**

Arbitrary Precision Library from GMP.  Link: https://gmplib.org
