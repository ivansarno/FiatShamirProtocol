using System;
using System.Numerics;
using System.Security.Cryptography;

namespace ZK_Fiat_Shamir
{
    public class Prime
    {
        private int _precision;
        private RandomNumberGenerator _generator;
        private byte[] _buffer;
        private BigInteger _current;

        public Prime(int precision = 20, int wordSize = 128)
        {
            if(precision < 5 || wordSize < 8)
                throw new ArgumentException();
            _precision = precision;
            _generator = new RNGCryptoServiceProvider();
            _buffer = new byte[wordSize];
            _current = 3;
        }

        public Prime(RandomNumberGenerator generator, int precision = 20, int wordSize = 128)
        {
            if (precision < 5 || wordSize < 8 || generator == null)
                throw new ArgumentException();
            _precision = precision;
            _generator = generator;
            _buffer = new byte[wordSize];
            _current = 3;
        }



        private static bool MRpredicate1(ref BigInteger y, ref BigInteger z, ref BigInteger number)
        {
            if (BigInteger.ModPow(y, z, number) == 1)
                return true;
            return false;
        }

        private static bool MRpredicate2(ref BigInteger y, ref BigInteger number, ref BigInteger z, int w)
        {
            int i = 0;
            bool cond = (BigInteger.ModPow(y, BigInteger.Pow(2, i) * z, number) == number - 1);

            while (!cond && i < w)
            {
                i++;
                cond = (BigInteger.ModPow(y, BigInteger.Pow(2, i) * z, number) == number - 1);
            }

            return i != w;
        }

        private bool MRtest(ref BigInteger number)  
        {
            int w = 0;
            BigInteger z = 0;

            MRscomposition(number, ref w, ref z);

            bool ris = true;
            int i = 0;


            BigInteger y;

            while (ris && i < _precision)
            {
                _generator.GetBytes(_buffer);
                _buffer[_buffer.Length - 1] &= 127;
                y = new BigInteger(_buffer);
                y = y % number;
                while (y < 2)
                {
                    _generator.GetBytes(_buffer);
                    _buffer[_buffer.Length - 1] &= 127;
                    y = new BigInteger(_buffer);
                    y = y % number;
                }
                ris = (BigInteger.GreatestCommonDivisor(y, number) == 1) && (MRpredicate1(ref y, ref z, ref number) || MRpredicate2(ref y, ref number, ref z, w));
                i++;
            }
            return ris;
        }

        private static void MRscomposition(BigInteger number, ref int w, ref BigInteger z)
        {
            int i = 1;
            BigInteger acc=2;
            number--;
            BigInteger r;
            while (acc < number)
            {
                r=number/acc;
                if ((number%acc==0) && (r%2==1))
                {
                    w = i;
                    z=r;
                }
                i++;
                acc = acc * 2;
            }
        }

        public bool IsPrime(ref BigInteger number)
        {
            if (number < 2)
                return false;
            return MRtest(ref number);
        }

        public BigInteger NextPrime(BigInteger number)
        {
            if (number % 2 == 0)
                number++;

            while (!MRtest(ref number ))//test primality
            {
                number += 2;
            }

            _current = number;
            return number;
        }

        public BigInteger NextPrime()
        {
            _current += 2;
            while (!MRtest(ref _current))//test primality
            {
                _current += 2;
            }

            return _current;
        }

        public BigInteger GetPrime()
        {
            _generator.GetBytes(_buffer);
            _buffer[_buffer.Length - 1] &= 127;
            BigInteger p = new BigInteger(_buffer);
            if (p % 2 == 0)
                p++;

            while (!MRtest(ref p))//test primality
            {
                p = p + 2;
            }

            _current = p;
            return p;
        }

        public void Reset(RandomNumberGenerator generator, int precision = 20, int wordSize = 128)
        {
            if (precision < 5 || wordSize < 8 || generator == null)
                throw new ArgumentException();
            _generator.Dispose();
            _precision = precision;
            _generator = generator;
            _buffer = new byte[wordSize];
            _current = 3;
        }

        public void Reset(int precision = 20, int wordSize = 128)
        {
            if (precision < 5 || wordSize < 8)
                throw new ArgumentException();
            _generator.Dispose();
            _precision = precision;
            _generator = new RNGCryptoServiceProvider();
            _buffer = new byte[wordSize]; 
            _current = 3;
        }
   }


}