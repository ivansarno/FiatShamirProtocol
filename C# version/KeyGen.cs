using System;
using System.Numerics;
using System.Security.Cryptography;

namespace ZK_Fiat_Shamir
{
	//object that creates and stores keys
    public class KeyGen
    {
        private BigInteger _module;
        private BigInteger _pubkey;
        private BigInteger _privkey;

		//check safety of selected prime numbers
        private bool Prime_check(ref BigInteger q, ref BigInteger p, ulong distance) 
        {
            BigInteger dif = (p - q);
            dif = BigInteger.Abs(dif);
            
            return (dif > distance);
        }
		
		//generates the keys
		//wordSize: number of bytes of the key
		//distance: distance between prime number, used for safety test
		//presision: precision of Miller-Rabin primality test
        public void KeyCreate(int wordSize = 128, ulong distance = UInt32.MaxValue, int precision = 20)
        {
            if (precision < 5 || wordSize < 8)
                throw new ArgumentException();

            RandomNumberGenerator gen = new RNGCryptoServiceProvider();
            Prime generator = new Prime(gen, precision, wordSize / 2);
            BigInteger primeP = generator.GetPrime();
            BigInteger primeQ = generator.GetPrime();

            while (!Prime_check(ref primeQ, ref primeP, distance)) //check safety of selected prime numbers
            {
                primeQ = generator.GetPrime();
            }

            _module = primeP*primeQ;
            
            byte[] buffer = new byte[wordSize];
            gen.GetBytes(buffer);
            buffer[buffer.Length - 1] &= 127; //forces a positive number
            _privkey = new BigInteger(buffer);
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) %_module;
            gen.Dispose();
        }
		
		//version with user's Random Number Generator
        public void KeyCreate(RandomNumberGenerator gen, int wordSize = 128, ulong distance = UInt32.MaxValue, int precision = 20)
        {
            if (precision < 5 || wordSize < 8 || gen == null)
                throw new ArgumentException();

            Prime generator = new Prime(gen, precision, wordSize / 2);
            BigInteger primeP = generator.GetPrime();
            BigInteger primeQ = generator.GetPrime();

            if (primeQ > primeP)
                while (!Prime_check(ref primeQ, ref primeP, distance))
                {
                    primeQ = generator.NextPrime();
                }
            else
                while (!Prime_check(ref primeQ, ref primeP, distance))
                {
                    primeP = generator.NextPrime(primeP);
                }

            _module = primeP * primeQ;

            byte[] buffer = new byte[wordSize];
            gen.GetBytes(buffer);
            buffer[buffer.Length - 1] &= 127;
            _privkey = new BigInteger(buffer);
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) % _module;
        }

        public BigInteger PrivKey
        {
            get { return _privkey; }
        }

        public BigInteger PubKey
        {
            get { return _pubkey; }
        }

        public BigInteger Module
        {
            get { return _module; }
        }
    }
}