//version V.2.1

using System;
using System.Numerics;
using System.Runtime.CompilerServices;

namespace ZK_Fiat_Shamir
{

    /// <summary>
    /// Utility for prime numbers.
    /// sequential version.
    /// for internal use, for now.
    /// </summary>
    internal class SeqPrime: IPrime
    {
        private readonly uint _precision; //precision of Miller-Rabin primality test
        private readonly Random _generator;
        private readonly byte[] _buffer; //used for random number generation


        /// <summary>
        ///  </summary>
        /// <param name="seed">seed of random number generator</param>
        /// <param name="precision">precision of Miller-Rabin test, error = 1/2^(2*precision)</param>
        /// <param name="wordSize">length in bytes of number generated</param>
        public SeqPrime(int seed, uint precision = 20, uint wordSize = 128)
        {
            if (precision < 5 || wordSize < 8)
                throw new ArgumentException("precision < 5 or wordSize < 8");
            _precision = precision;
            _generator = new Random(seed);
            _buffer = new byte[wordSize];
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
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
                cond = (BigInteger.ModPow(y, pow2*z, number) == number - 1);
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


            while (ris && i < _precision)
            {
                //extract a random number
                _generator.NextBytes(_buffer);
                _buffer[_buffer.Length - 1] &= 127; //forces a positive number
                var y = new BigInteger(_buffer);
                ////
                y = y%number;
                while (y < 2) //avoids extraction of 0 and 1
                {
                    y += _generator.Next();
                    y = y%number;
                }
                //test
                ris = (BigInteger.GreatestCommonDivisor(y, number) == 1) &&
                      (MRpredicate1(ref y, ref z, ref number) || MRpredicate2(ref y, ref number, ref z, w));
                i++;
            }
            return ris;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void MRscomposition(ref BigInteger number, out uint w, out BigInteger z)
        {
            z = number - 1;
            w = 0;
            while ((z & 1) == 0)
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
            return (number == 2) || (number > 2 && MRtest(ref number));
        }

        /// <summary>
        /// Return the first prime number following the argument.
        /// </summary>
        /// <param name="number">current number</param>
        /// <returns>next prime number</returns>
        public BigInteger NextPrime(BigInteger number)
        {
            if (number < 2)
                return 2;
            if ((number & 1) == 0)
                number++;

            while (!MRtest(ref number)) //test primality
            {
                number += 2;
            }

            return number;
        }


        /// <summary>
        /// Version to use with threads.
        /// On termination current contains next prime number.
        /// </summary>
        /// <param name="current">current number</param>
        /// <returns>next prime number</returns>
        public BigInteger NextPrime(object current)
        {
            var number = (BigInteger) current;
            if (number < 2)
                return 2;
            if ((number & 1) == 0)
                number++;

            while (!MRtest(ref number)) //test primality
            {
                number += 2;
            }

            return number;
        }
    }
}
