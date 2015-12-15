//version V.2.0

using System;
using System.Numerics;
using System.Security.Cryptography;


namespace ZK_Fiat_Shamir
{
    public class Proover 
    {
        private readonly BigInteger _key;
        private readonly BigInteger _mod;
        private BigInteger _sessionNumber;
        private readonly GeneratorWrap _generator;
        private bool _synch;


        /// <summary>
        /// </summary>
        /// <param name="privateKey">private key</param>
        /// <param name="module">module of the key</param>
        /// <param name="wordSize">key size in bytes</param>
        /// <param name="gen">random number generator, it is not disposed.</param>
        public Proover(BigInteger privateKey, BigInteger module, RandomNumberGenerator gen, uint wordSize = 128)
        {
            if (module <= 1 || wordSize < 8 || gen == null)
                throw new ArgumentException("module <= 1 or wordSize < 8 or gen == null");
            _mod = module;
            _key = privateKey;
            _generator = new GeneratorWrap(gen, wordSize);
            _synch = false;
        }


        /// <summary>
        /// Start the protocol and return an init for Verifier.Step1.
        /// </summary>
        /// <returns>number to send to Verifier</returns>
        public BigInteger Step1()
        {

            _sessionNumber = _generator.GetBig()%_mod;
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
    }
}
