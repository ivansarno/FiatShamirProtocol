//version V.1.5

using System;
using System.Numerics;
using System.Security.Cryptography;


namespace ZK_Fiat_Shamir
{
    
    /// <summary>
    /// Object that creates and stores keys.
    /// </summary>
    public class KeyGen
    {
        private BigInteger _module;
        private BigInteger _pubkey;
        private BigInteger _privkey;
        private uint _keySize;

		//check safety of selected prime numbers
        private static bool Prime_check(ref BigInteger q, ref BigInteger p, ulong distance) 
        {
            BigInteger dif = (p - q);
            dif = BigInteger.Abs(dif);
            
            return (dif > distance);
        }


        /// <summary>
        /// Generates the keys.
        /// </summary>
        /// <param name="wordSize">number of bytes of the key</param>
        /// <param name="distance">distance between prime number, used for safety test</param>
        /// <param name="precision">precision of Miller-Rabin primality test, error = 1/2^(2*precision)</param>
        public void KeyCreate(uint wordSize = 128, ulong distance = uint.MaxValue, uint precision = 20)
        {
            if (precision < 1 || wordSize < 8)
                throw new ArgumentException("precision < 1 or wordSize < 8");

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
            _keySize = wordSize;
        }


        /// <summary>
        /// Generates the keys.
        /// Version with user's Random Number Generator, it is not disposed.
        /// </summary>
        /// <param name="gen">random number generator</param>
        /// <param name="wordSize">number of bytes of the key</param>
        /// <param name="distance">distance between prime number, used for safety test</param>
        /// <param name="precision">precision of Miller-Rabin primality test, error = 1/2^(2*precision)</param>
        public void KeyCreate(RandomNumberGenerator gen, uint wordSize = 128, ulong distance = uint.MaxValue, uint precision = 20)
        {
            if (precision < 1 || wordSize < 8 || gen == null)
                throw new ArgumentException("precision < 1 or wordSize < 8");

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
            _keySize = wordSize;
        }

        public BigInteger PrivateKey => _privkey;

        public BigInteger PublicKey => _pubkey;

        public BigInteger Module => _module;

        public uint KeySize => _keySize;
    }
}