using System;
using System.Numerics;

namespace ZK_Fiat_Shamir
{
    class Verifier
    {
        private bool _choice;
        private readonly BigInteger _key;
        private readonly BigInteger _mod;
        private BigInteger _sessionNumber;
        private bool _state;
        private Random _bitgen;

        public Verifier(BigInteger pubkey, BigInteger module)
        {
            if (module <= 0)
                throw new ArgumentException();
            _key = pubkey;
            _mod = module;
            _state = false;
            _bitgen = new Random();
        }

        public bool Step1(ref BigInteger init)
        {
            if (init == 0)
                throw new ArgumentException();

            _sessionNumber = init;
            _choice = (_bitgen.Next() % 2) == 1;
            return _choice;
        }

        public bool Step2(BigInteger proof)
        {
            proof = (proof * proof) % _mod;

            BigInteger y;

            if (_choice)
                y = (_sessionNumber * _key) % _mod;
            else y = _sessionNumber;

            _state = proof == y;

            return _state;
        }

        public bool Checkstate()
        {
            return _state;
        }

        public void Reset()
        {
            _state = false;
            _bitgen = new Random();
        }
    }
}
