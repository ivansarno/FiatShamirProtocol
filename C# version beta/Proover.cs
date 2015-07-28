using System;
using System.Numerics;
using System.Security.Cryptography;


namespace ZK_Fiat_Shamir
{
    class Proover
    {
        private readonly BigInteger _key;
        private readonly BigInteger _mod;
        private BigInteger _sessionNumber;
        private RandomNumberGenerator _generator;
        private readonly byte[] _buffer;

        public Proover(BigInteger privkey, BigInteger module, int wordSize = 128)
        {
            if (module <= 0 || wordSize < 8)
                throw new ArgumentException();

            _mod = module;
            _key = privkey;
            _generator = new RNGCryptoServiceProvider();
            _buffer = new byte[wordSize];
        }
		
		//version with user's random number generator
        public Proover(BigInteger privkey, BigInteger module, RandomNumberGenerator gen, int wordSize = 128)
        {
            if (module <= 0 || wordSize < 8 || gen == null)
                throw new ArgumentException();
            _mod = module;
            _key = privkey;
            _generator = gen;
            _buffer = new byte[wordSize];
        }
		
		//start the protocol
        public BigInteger Step1()
        {
            _generator.GetBytes(_buffer);
            _buffer[_buffer.Length - 1] &= 127;
            _sessionNumber = new BigInteger(_buffer);
            _sessionNumber %= _mod;//1
            return (_sessionNumber * _sessionNumber) % _mod;
        }

		//take the result of Verifier.step1() and return a number to send to Verifier
        public BigInteger Step2(bool choice)
        {
            if (choice)
                return (_sessionNumber * _key) % _mod;
            return _sessionNumber;
        }

        public void Reset()
        {
            _generator.Dispose();
            _generator = new RNGCryptoServiceProvider();
        }

		//version with user's random number generator
        public void Reset(RandomNumberGenerator gen)
        {
            if (gen == null)
                throw new ArgumentException();
            _generator.Dispose();
            _generator = gen;
        }
    }
}
