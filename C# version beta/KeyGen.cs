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


        private bool Prime_check(ref BigInteger q, ref BigInteger p, ulong distance) 
        {
            BigInteger dif = (p - q);
            dif = BigInteger.Abs(dif);
            
            return (dif > distance);
        }

        public void KeyCreate(int wordSize = 128, ulong distance = UInt32.MaxValue, int precision = 20)
        {
            if (precision < 5 || wordSize < 8)
                throw new ArgumentException();

            RandomNumberGenerator gen = new RNGCryptoServiceProvider();
            Prime generator = new Prime(gen, precision, wordSize / 2);
            BigInteger primeP = generator.GetPrime();
            BigInteger primeQ = generator.GetPrime();

            while (!Prime_check(ref primeQ, ref primeP, distance))
            {
                primeQ = generator.GetPrime();
            }

            _module = primeP*primeQ;
            
            byte[] buffer = new byte[wordSize];
            gen.GetBytes(buffer);
            buffer[buffer.Length - 1] &= 127;
            _privkey = new BigInteger(buffer);
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) %_module;
            gen.Dispose();
        }

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