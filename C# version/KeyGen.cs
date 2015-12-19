//version V.2.1

using System;
using System.Numerics;
using System.Security.Cryptography;
using System.Threading;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;


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
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static bool PrimeCheck(ref BigInteger q, ref BigInteger p, ulong distance) 
        {
            return BigInteger.Abs(p-q) > distance;
        }


        /// <summary>
        /// Generates the keys.
        /// </summary>
        /// <param name="gen">random number generator, it is not disposed.</param>
        /// <param name="wordSize">number of bytes of the key</param>
        /// <param name="distance">distance between prime number, used for safety test</param>
        /// <param name="precision">precision of Miller-Rabin primality test, error = 1/2^(2*precision)</param>
        public void KeyCreate(RandomNumberGenerator gen, uint wordSize = 128, ulong distance = uint.MaxValue, uint precision = 20)
        {

            if (precision < 1 || wordSize < 8 || gen == null)
                throw new ArgumentException("precision < 1 or wordSize < 8 or gen == null");

            var genwrap = new GeneratorWrap(gen, wordSize/2);
            _keySize = wordSize;
            SeqPrime generator = new SeqPrime(genwrap.GetInt(), precision, wordSize / 2);
            BigInteger primeP = generator.NextPrime(genwrap.GetBig());
            BigInteger primeQ = generator.NextPrime(genwrap.GetBig());

            while (!PrimeCheck(ref primeQ, ref primeP, distance)) //check safety of selected prime numbers
             {
                 primeQ = generator.NextPrime(genwrap.GetBig());
             }

            genwrap = new GeneratorWrap(gen, wordSize);
            //key creation
            Creation(ref primeP, ref primeQ, genwrap);
        }

        public BigInteger PrivateKey => _privkey;

        public BigInteger PublicKey => _pubkey;

        public BigInteger Module => _module;

        public uint KeySize => _keySize;

        //Creates the keys
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void Creation(ref BigInteger primeP, ref BigInteger primeQ, GeneratorWrap gen)
        {
            _module = primeP * primeQ;

            _privkey = gen.GetBig();
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) % _module;
        }

        /// <summary>
        /// Generates the keys.
        /// Multithread version.
        /// </summary>
        /// <param name="gen">random number generator, it is not disposed.</param>
        /// <param name="wordSize">key size in bytes</param>
        /// <param name="distance">distance between prime numbers, used for safety test</param>
        /// <param name="precision">precision of Miller-Rabin primality test, error = 1/2^(2*precision)</param>
        /// <param name="threads">number of threads to use.</param>
        public void ParallelKeyCreate(RandomNumberGenerator gen, uint wordSize = 128, ulong distance = uint.MaxValue,
            uint precision = 20, int threads = 4)
        {
           if(threads < 2)
                KeyCreate(gen, wordSize, distance, precision);

            if (precision < 1 || wordSize < 8 || gen == null)
                throw new ArgumentException("precision < 1 or wordSize < 8 or gen == null");

            _keySize = wordSize;
            var genwrap = new GeneratorWrap(gen, wordSize/2);//generator half size
            IPrime mainGenerator, workerGenerator;

            //threads' distribution for primes creation 
            if (threads < 4)
            {
                mainGenerator = new SeqPrime(genwrap.GetInt(), precision, wordSize/2);
                workerGenerator = new SeqPrime(genwrap.GetInt(), precision, wordSize/2);
            }
            else
            {
                mainGenerator = new ParPrime(genwrap.GetInt(), precision, wordSize/2, threads-threads/2);
                workerGenerator = new ParPrime(genwrap.GetInt(), precision, wordSize / 2, threads/2);
            }

            //primes creation
            var number = genwrap.GetBig();
            Task<BigInteger> worker = new Task<BigInteger>(workerGenerator.NextPrime, number);
            worker.Start();
            var primeP = mainGenerator.NextPrime(genwrap.GetBig());

            worker.Wait();
            var primeQ = worker.Result;
            worker.Dispose();

            // prime numbers security check
            if (!PrimeCheck(ref primeP, ref primeQ, distance))
            {
                //use all threads
                mainGenerator = new ParPrime(genwrap.GetInt(), precision, wordSize / 2, threads);
                do
                {
                    primeQ = mainGenerator.NextPrime(genwrap.GetBig());
                } while (!PrimeCheck(ref primeQ, ref primeP, distance)); // prime numbers security check
            }

            //generator full size
            genwrap = new GeneratorWrap(gen, wordSize);
            //key creation
            Creation(ref primeP, ref primeQ, genwrap);
        }
    }
}