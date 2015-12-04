//version V.1.1

using System;
using System.Numerics;
using System.Security.Cryptography;

namespace ZK_Fiat_Shamir
{

    /// <summary>
    /// Utility for prime numbers.
    /// </summary>
    public class Prime : IDisposable
    {
        private readonly uint _precision; //precision of Miller-Rabin primality test
        private readonly RandomNumberGenerator _generator;
        private readonly byte[] _buffer; //used for random number generation
        private BigInteger _current; //current prime number, by default is 3
        private bool _disposed;
        
        /// <summary>
        /// Version with built-in random number generator.
        /// </summary>
        /// <param name="precision">precision of Miller-Rabin test, error = 1/2^(2*precision)</param>
        /// <param name="wordSize">length in bytes of number generated</param>
        public Prime(uint precision = 20, uint wordSize = 128)
        {
            if(precision < 5 || wordSize < 8)
                throw new ArgumentException("precision < 5 or wordSize < 8");
            _precision = precision;
            _generator = new RNGCryptoServiceProvider();
            _buffer = new byte[wordSize];
            _current = 3;
        }


        /// <summary>
        /// Version with user's random number generator, it is not disposed.
        /// </summary>
        /// <param name="generator">random number generator</param>
        /// <param name="precision">precision of Miller-Rabin test, error = 1/2^(2*precision)</param>
        /// <param name="wordSize">length in bytes of number generated</param>
        public Prime(RandomNumberGenerator generator, uint precision = 20, uint wordSize = 128)
        {
            if (precision < 5 || wordSize < 8 || generator == null)
                throw new ArgumentException("precision < 5 or wordSize < 8 or gen == null");
            _precision = precision;
            _generator = generator;
            _buffer = new byte[wordSize];
            _current = 3;
        }



        private static bool MRpredicate1(ref BigInteger y, ref BigInteger z, ref BigInteger number)
        {
            return BigInteger.ModPow(y, z, number) == 1;
        }

        private static bool MRpredicate2(ref BigInteger y, ref BigInteger number, ref BigInteger z, uint w)
        {
            uint i = 0;
            BigInteger pow2 = 1;
            bool cond = (BigInteger.ModPow(y, z, number) == number - 1);

            while (!cond && i < w)
            {
                i++;
                pow2 <<= 1;
                cond = (BigInteger.ModPow(y, pow2 * z, number) == number - 1);
            }

            return i != w;
        }

        private bool MRtest(ref BigInteger number)
        {
            uint w;
            BigInteger z;

            MRscomposition(ref number, out w, out z);

            bool ris = true;
            uint i = 0;


            BigInteger y;

            while (ris && i < _precision)
            {
                _generator.GetBytes(_buffer);
                _buffer[_buffer.Length - 1] &= 127; //forces a positive number
                y = new BigInteger(_buffer);
                y = y % number;
                while (y < 2) //avoids extraction of 0 and 1
                {
                    _generator.GetBytes(_buffer);
                    _buffer[_buffer.Length - 1] &= 127;
                    y = new BigInteger(_buffer);
                    y = y % number;
                }
				//test
                ris = (BigInteger.GreatestCommonDivisor(y, number) == 1) && (MRpredicate1(ref y, ref z, ref number) || MRpredicate2(ref y, ref number, ref z, w));
                i++;
            }
            return ris;
        }

        private static void MRscomposition(ref BigInteger number, out uint w, out BigInteger z)
        {
            z = number - 1;
            w = 0;
            while ((z&1) == 0)
            {
                w++;
                z >>= 1;
            }
        }


        /// <summary>
        /// Primality test.
        /// </summary>
        /// <param name="number">number to test</param>
        /// <returns>true if number is prime</returns>
        public bool IsPrime(ref BigInteger number)
        {
            return number >= 2 && MRtest(ref number);
        }

        /// <summary>
        /// Return the first number following the argument.
        /// </summary>
        /// <param name="number">current number</param>
        /// <returns>next prime number</returns>
        public BigInteger NextPrime(BigInteger number)
        {
            if ((number & 1) == 0)
                number++;

            while (!MRtest(ref number ))//test primality
            {
                number += 2;
            }

            _current = number;
            return number;
        }

        /// <summary>
        /// Return the first number following that retrieved the last time.
        /// </summary>
        /// <returns>next prime number</returns>
        public BigInteger NextPrime()
        {
            _current += 2;
            while (!MRtest(ref _current))//test primality
            {
                _current += 2;
            }

            return _current;
        }


        /// <summary>
        /// Generates a prime number.
        /// </summary>
        /// <returns>Random prime number.</returns>
        public BigInteger GetPrime()
        {
            _generator.GetBytes(_buffer);
            _buffer[_buffer.Length - 1] &= 127;
            BigInteger p = new BigInteger(_buffer);
            if ((p & 1) == 0)
                p++;

            while (!MRtest(ref p))//test primality
            {
                p = p + 2;
            }

            _current = p;
            return p;
        }

        //IDisposable pattern implementation
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (_disposed)
                return;

            if (disposing)
            {
                _generator.Dispose();
            }

            _disposed = true;
        }
    }

}