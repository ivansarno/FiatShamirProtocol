//version V.2.0

using System;
using System.Numerics;
using System.Security.Cryptography;
using System.Threading;


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
            Prime generator = new Prime(genwrap.GetInt(), precision, wordSize / 2);
            BigInteger primeP = generator.NextPrime(genwrap.GetBig());
            BigInteger primeQ = generator.NextPrime(genwrap.GetBig());

            while (!Prime_check(ref primeQ, ref primeP, distance)) //check safety of selected prime numbers
             {
                 primeQ = generator.NextPrime(genwrap.GetBig());
             }

            genwrap = new GeneratorWrap(gen, wordSize);
            Creation(ref primeP, ref primeQ, genwrap);
        }

        public BigInteger PrivateKey => _privkey;

        public BigInteger PublicKey => _pubkey;

        public BigInteger Module => _module;

        public uint KeySize => _keySize;

        //Creates the keys
        private void Creation(ref BigInteger primeP, ref BigInteger primeQ, GeneratorWrap gen)
        {
            _module = primeP * primeQ;

            _privkey = gen.GetBig();
            _privkey %= _module;
            _pubkey = (_privkey * _privkey) % _module;
        }

        /// <summary>
        /// Generates the keys.
        /// Mutltithread version.
        /// Now support 2 threads only.
        /// </summary>
        /// <param name="gen">random number generator, it is not disposed.</param>
        /// <param name="wordSize">key size in bytes</param>
        /// <param name="distance">distance between prime numbers, used for safety test</param>
        /// <param name="precision">precision of Miller-Rabin primality test, error = 1/2^(2*precision)</param>
        /// <param name="threads">number of threads to use.</param>
        public void ParallelKeyCreate(RandomNumberGenerator gen, uint wordSize = 128, ulong distance = uint.MaxValue,
            uint precision = 20, int threads = 4)
        {
            if (precision < 1 || wordSize < 8 || gen == null)
                throw new ArgumentException("precision < 1 or wordSize < 8 or gen == null");
            
            if(threads < 2)
                KeyCreate(gen, wordSize, distance, precision);
                
            _keySize = wordSize;
            var genwrap = new GeneratorWrap(gen, wordSize/2);
        
            BigInteger primeP, primeQ;
            
            if(threads < 4)
                DualRoutine(precision, wordSize, genwrap, distance, out primeP, out primeQ);
            else
                ParallelRoutine(precision, wordSize, genwrap, threads, distance, out primeP, out primeQ);

            genwrap = new GeneratorWrap(gen, wordSize);
            Creation(ref primeP, ref primeQ, genwrap);
        }
        
        //routine for 2 threads.
        private static void DualRoutine(uint precision, uint wordSize, GeneratorWrap gen, ulong distance, out BigInteger primeP, out BigInteger primeQ)
        {
            var number = gen.GetBig();
            var container = new Container(number);
            var seeker = new Prime(gen.GetInt(), precision, wordSize/2);
            Thread worker = new Thread(() => seeker.NextPrime(container));
            worker.Start();

            var generator = new Prime(gen.GetInt(), precision, wordSize/2);
            number = gen.GetBig();
            primeP = generator.NextPrime(number);
            
            worker.Join();
            primeQ = container.Content;

            while (!Prime_check(ref primeQ, ref primeP, distance)) // prime numbers security check
            {
                primeQ = generator.ParallelNextPrime(gen.GetBig());
            }
        }
        
        //routine for threads > 2
        private static void ParallelRoutine(uint precision, uint wordSize, GeneratorWrap gen, int threads, ulong distance, out BigInteger primeP, out BigInteger primeQ)
        {

            var number = gen.GetBig();
            var container = new Container(number);
            var seeker = new Prime(gen.GetInt(), precision, wordSize / 2);
            Thread worker = new Thread(() => seeker.ParallelNextPrime(container, threads/2));
            worker.Start();

            var generator = new Prime(gen.GetInt(), precision, wordSize / 2);
            number = gen.GetBig();
            primeP = generator.ParallelNextPrime(number, threads/2);

            worker.Join();
            primeQ = container.Content;

            while (!Prime_check(ref primeQ, ref primeP, distance)) //check safety of selected prime numbers
            {
                primeQ = generator.ParallelNextPrime(gen.GetBig(), threads);
            }
        }
    }
}