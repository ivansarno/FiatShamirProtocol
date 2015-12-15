//version V.2.0

using System;
using System.Numerics;
using System.Threading;

namespace ZK_Fiat_Shamir
{

    /// <summary>
    /// Utility for prime numbers.
    /// for internal use, for now.
    /// </summary>
    internal class Prime
    {
        private readonly uint _precision; //precision of Miller-Rabin primality test
        private readonly Random _generator;
        private readonly byte[] _buffer; //used for random number generation


        /// <summary>
        ///  </summary>
        /// <param name="seed">seed of random number generator</param>
        /// <param name="precision">precision of Miller-Rabin test, error = 1/2^(2*precision)</param>
        /// <param name="wordSize">length in bytes of number generated</param>
        public Prime(int seed, uint precision = 20, uint wordSize = 128)
        {
            if (precision < 5 || wordSize < 8)
                throw new ArgumentException("precision < 5 or wordSize < 8");
            _precision = precision;
            _generator = new Random(seed);
            _buffer = new byte[wordSize];
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


            BigInteger y;

            while (ris && i < _precision)
            {
                //extract a random number
                _generator.NextBytes(_buffer);
                _buffer[_buffer.Length - 1] &= 127; //forces a positive number
                y = new BigInteger(_buffer);
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
        /// <param name="current">container of current number</param>
        public void NextPrime(Container current)
        {
            if (current.Content < 2)
            {
                current.Content = 2;
                return;
            }
            var number = current.Content;
            if ((number & 1) == 0)
                number++;

            while (!MRtest(ref number)) //test primality
            {
                number += 2;
            }

            current.Content = number;
        }

        /// <summary>
        /// Return the first prime number following the argument.
        /// Multithread version.
        /// Now supports only 2 threads
        /// </summary>
        /// <param name="number">current number</param>
        /// <param name="threads">number of threads to use</param>
        /// <returns>next prime number</returns>
        public BigInteger ParallelNextPrime(BigInteger number, int threads = 2)
        {
            if (number < 2)
                return 2;
            if (threads < 2)
                return NextPrime(number);

            if ((number & 1) == 0)
                number++;

            var wait = new WaitHandle[]
            {
                new AutoResetEvent(false), new AutoResetEvent(false)
            };
            var result1 = new Container(number);
            var worker1 = new Thread(() => Routine(result1, wait[0]));
            worker1.Start();
            var result2 = new Container(number+2);
            var worker2 = new Thread(new ThreadStart(() => Routine(result2, wait[1])));////////////////////
            worker2.Start();

            var index =  WaitHandle.WaitAny(wait);
            if (index == 0)
            {
                worker2.Abort();
                return result1.Content;
            }
            else
            {
                worker1.Abort();
                return result2.Content;
            }

        }

        /// <summary>
        /// Version to use with threads.
        /// On termination current contains next prime number.
        /// Multithread version.
        /// Now supports only 2 threads.
        /// </summary>
        /// <param name="current">container of current number</param>
        /// <param name="threads">number of threads to use</param>
        public void ParallelNextPrime(Container current, int threads = 2)
        {
            if (current.Content < 2)
            {
                current.Content = 2;
                return;
            }
            if (threads < 2)
                NextPrime(current);

            var number = current.Content;
            if ((number & 1) == 0)
                number++;
            var wait = new WaitHandle[]
            {
                new AutoResetEvent(false), new AutoResetEvent(false)
            };
            var result1 = new Container(number);
            var worker1 = new Thread(new ThreadStart(() => Routine(result1, wait[0])));
            worker1.Start();
            var result2 = new Container(number + 2);
            var worker2 = new Thread(new ThreadStart(() => Routine(result2, wait[1])));
            worker2.Start();

            var index = WaitHandle.WaitAny(wait);
            if (index == 0)
            {
                worker2.Abort();
                current.Content = result1.Content;
            }
            else
            {
                worker1.Abort();
                current.Content = result2.Content;
            }

        }

        //thread's routine
        private void Routine(Container number, WaitHandle wait)
        {
            var seed = number.Content;
            try
            {
                while (!MRtest(ref seed))
                    seed += 4;
                number.Content = seed;
               ((AutoResetEvent) wait).Set();
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
            }
        }
}
}
