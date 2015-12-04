//version V.1.5

using System;
using System.Numerics;
using System.Security.Cryptography;


namespace ZK_Fiat_Shamir
{
    public class Proover : IDisposable
    {
        private readonly BigInteger _key;
        private readonly BigInteger _mod;
        private BigInteger _sessionNumber;
        private readonly RandomNumberGenerator _generator;
        private readonly byte[] _buffer;
        private bool _synch;
        private bool _disposed;

        /// <summary>
        /// Version with built-in random number generator.
        /// </summary>
        /// <param name="privateKey">private key</param>
        /// <param name="module">module of the kay</param>
        /// <param name="wordSize">number of bytes of the key</param>
        public Proover(BigInteger privateKey, BigInteger module, uint wordSize = 128)
        {
            if (module <= 1 || wordSize < 8)
                throw new ArgumentException("module <= 1 or wordSize < 8");

            _mod = module;
            _key = privateKey;
            _generator = new RNGCryptoServiceProvider();
            _buffer = new byte[wordSize];
            _synch = false;
        }


        /// <summary>
        /// Version with user's random number generator, it is not disposed.
        /// </summary>
        /// <param name="privateKey">private key</param>
        /// <param name="module">module of the kay</param>
        /// <param name="wordSize">number of bytes of the key</param>
        /// <param name="gen">random number generator</param>
        public Proover(BigInteger privateKey, BigInteger module, RandomNumberGenerator gen, uint wordSize = 128)
        {
            if (module <= 1 || wordSize < 8 || gen == null)
                throw new ArgumentException("module <= 1 or wordSize < 8 or gen == null");
            _mod = module;
            _key = privateKey;
            _generator = gen;
            _buffer = new byte[wordSize];
            _synch = false;
        }


        /// <summary>
        /// Start the protocol and return a init for Verifier.Step1.
        /// </summary>
        /// <returns>number to send to Verifier</returns>
        public BigInteger Step1()
        {
            _generator.GetBytes(_buffer);
            _buffer[_buffer.Length - 1] &= 127;
            _sessionNumber = new BigInteger(_buffer);
            _sessionNumber %= _mod;
            _synch = true;
            while (_sessionNumber < 2) //avoid comunication of the key
                _sessionNumber = (_sessionNumber + 2)%_mod;
            return (_sessionNumber*_sessionNumber)%_mod;

        }


        /// <summary>
        /// Take the result of Verifier.step1() and return the proof to send to Verifier.
        /// </summary>
        /// <param name="choice">result of Verifier.step1()</param>
        /// <returns>a number to send to Verifier</returns>
        public BigInteger Step2(bool choice)
        {
            if (_synch)
                _synch = false;
            else throw new InvalidOperationException("Called Proover.Step2 before calling Proover.Step1");
            if (choice)
                return (_sessionNumber*_key)%_mod;
            return _sessionNumber;
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
