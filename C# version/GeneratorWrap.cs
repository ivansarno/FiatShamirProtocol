//version V.2.0

using System;
using System.Numerics;
using System.Security.Cryptography;


namespace ZK_Fiat_Shamir
{
    internal class GeneratorWrap
    {
        private readonly RandomNumberGenerator _gen;
        private readonly byte[] _buffer;
        public uint Size { get;}

        public GeneratorWrap(RandomNumberGenerator generator, uint wordSize)
        {
            Size = wordSize;
            _gen = generator;
            _buffer = new byte[wordSize];
        }

        public BigInteger GetBig()
        {
            _gen.GetBytes(_buffer);
            _buffer[Size-1] &= 127; //forces a positive number
            return new BigInteger(_buffer);
        }

        public long GetLong()
        {
            _gen.GetBytes(_buffer, 0, 8);
            return BitConverter.ToInt64(_buffer, 0);
        }

        public int GetInt()
        {
            _gen.GetBytes(_buffer, 0, 4);
            return BitConverter.ToInt32(_buffer, 0);
        }
    }
}
