using System;
using System.Numerics;
using System.Security.Cryptography;

namespace ZK_Fiat_Shamir
{
    public class KeyGen
    {
        private BigInteger _module;
        private BigInteger _pubkey;
        private BigInteger _privkey;


        private bool Prime_check(ref BigInteger q, ref BigInteger p, int distance) 
        {
            BigInteger dif = (p - q);
            BigInteger.Abs(dif);
            
            return (dif > distance);
        }

        public void KeyCreate(int wordSize = 128, int distance = UInt16.MaxValue, int precision = 20)
        {
            Prime generator = new Prime(precision, wordSize);
            BigInteger primeP = generator.GetPrime();
            BigInteger primeQ = generator.GetPrime();

            if(primeQ > primeP)
                while (!Prime_check(ref primeQ, ref primeP, distance))
                {
                    primeQ = generator.NextPrime();
                }
            else
                while (!Prime_check(ref primeQ, ref primeP, distance))
                {
                    primeP = generator.NextPrime(primeP);
                }

            _module = primeP*primeQ;
            
            byte[] buffer = new byte[wordSize];
            RandomNumberGenerator gen = new RNGCryptoServiceProvider();
            gen.GetBytes(buffer);
            buffer[buffer.Length - 1] &= 127;
            _privkey = new BigInteger(buffer);
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) %_module;
            gen.Dispose();
        }

        public void KeyCreate(RandomNumberGenerator randomGenerator, int wordSize = 128, int distance = UInt16.MaxValue, int precision = 20)
        {
            Prime generator = new Prime(randomGenerator, precision, wordSize);
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
            RandomNumberGenerator gen = new RNGCryptoServiceProvider();
            gen.GetBytes(buffer);
            buffer[buffer.Length - 1] &= 127;
            _privkey = new BigInteger(buffer);
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) % _module;
            gen.Dispose();
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