//version V.2.0

using System.Numerics;
using System.Security.Cryptography;

namespace ZK_Fiat_Shamir
{
    public class Test
    {
        /// <summary>
        /// Simple test.
        /// </summary>
        /// <param name="wordSize">number of byte of the key</param>
        /// <param name="testPrecision">percision of the test, error = 1/2^precision</param>
        /// <returns>resoult of test</returns>
        public static bool DefaultTest(uint wordSize=128, uint testPrecision=20)
        {
            if (wordSize < 8 || testPrecision < 1)
            {
                System.Console.WriteLine("ZKFS test invalid input\n");
                return false;
            }

            uint iteration = 0;
            bool ran;
            BigInteger com;
            bool result = true;
            KeyGen kg = new KeyGen();
            var gen = new RNGCryptoServiceProvider();
            kg.KeyCreate(gen, wordSize);
            Verifier v = new Verifier(kg.PublicKey, kg.Module);
            Proover p = new Proover(kg.PrivateKey, kg.Module, gen);
            
            //test with key
            while (iteration < testPrecision && result)
            {
                com = p.Step1();
                ran = v.Step1(ref com);
                com = p.Step2(ran);
                v.Step2(com);
                result = v.Checkstate();
                iteration++;
            }

            if (!result) //if not verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                gen.Dispose();
                return false;
            }

            
            //test without key
            BigInteger falseKey = kg.PrivateKey - (kg.PrivateKey/3); 
            p = new Proover(falseKey, kg.Module, gen);
            iteration = 0;
            while (iteration < testPrecision && result)
            {
                com = p.Step1();
                ran = v.Step1(ref com);
                com = p.Step2(ran);
                v.Step2(com);
                result = v.Checkstate();
                iteration++;
            }

            if (result) //if verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                gen.Dispose();
                return false;
            }
            
            System.Console.WriteLine("ZKFS test OK\n");
            gen.Dispose();
            return true;
        }


        /// <summary>
        /// Customizable version with full KeyGen parameters.
        /// </summary>
        /// <param name="wordSize">number of byte of the key</param>
        /// <param name="testPrecision">percision of the test, error = 1/2^precision</param>
        /// <param name="primePrecision">percision of primality test, error = 1/2^(2*precision)</param>
        /// <param name="generator">random number generator, it is not disposed</param>
        /// <param name="primeDistance">distance between 2 prime, for security</param>
        /// <param name="threads">number of threads to use</param>
        /// <returns>resoult of test</returns>
        public static bool CustomTest(uint wordSize, uint testPrecision, uint primePrecision, RandomNumberGenerator generator, ulong primeDistance=uint.MaxValue, int threads = 0)
        {
            if (wordSize < 8 || testPrecision < 1)
            {
                System.Console.WriteLine("ZKFS test invalid input\n");
                return false;
            }

            uint iteration = 0;
            bool ran;
            BigInteger com;
            bool result = true;
            KeyGen kg = new KeyGen();
            kg.ParallelKeyCreate(generator, wordSize, primeDistance, primePrecision, threads);
            Verifier v = new Verifier(kg.PublicKey, kg.Module);
            Proover p = new Proover(kg.PrivateKey, kg.Module, generator);

            //test with key
            while (iteration < testPrecision && result)
            {
                com = p.Step1();
                ran = v.Step1(ref com);
                com = p.Step2(ran);
                v.Step2(com);
                result = v.Checkstate();
                iteration++;
            }

            if (!result) //if not verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                return false;
            }

            //test without key
            BigInteger falseKey = kg.PrivateKey - (kg.PrivateKey / 3);
            p = new Proover(falseKey, kg.Module, generator);
            iteration = 0;
            while (iteration < testPrecision && result)
            {
                com = p.Step1();
                ran = v.Step1(ref com);
                com = p.Step2(ran);
                v.Step2(com);
                result = v.Checkstate();
                iteration++;
            }

            if (result) //if verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                return false;
            }

            System.Console.WriteLine("ZKFS test OK\n");
            return true;
        }
    }
}
