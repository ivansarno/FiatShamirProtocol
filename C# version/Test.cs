//version V.1.5

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
            if (wordSize < 64 || testPrecision < 1)
            {
                System.Console.WriteLine("ZKFS test invalid input\n");
                return false;
            }

            uint iteration = 0;
            bool ran;
            BigInteger com;
            bool result = true;
            KeyGen kg = new KeyGen();
            kg.KeyCreate(wordSize);
            Verifier V = new Verifier(kg.PublicKey, kg.Module);
            Proover P = new Proover(kg.PrivateKey, kg.Module);

            //test with key
            while (iteration < testPrecision && result)
            {
                com = P.Step1();
                ran = V.Step1(ref com);
                com = P.Step2(ran);
                V.Step2(com);
                result = V.Checkstate();
                iteration++;
            }

            if (!result) //if not verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                P.Dispose();
                return false;
            }

            P.Dispose();
            //test without key
            BigInteger falseKey = kg.PrivateKey - (kg.PrivateKey/3); 
            P = new Proover(falseKey, kg.Module);
            iteration = 0;
            while (iteration < testPrecision && result)
            {
                com = P.Step1();
                ran = V.Step1(ref com);
                com = P.Step2(ran);
                V.Step2(com);
                result = V.Checkstate();
                iteration++;
            }

            if (result) //if verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                P.Dispose();
                return false;
            }
            
            System.Console.WriteLine("ZKFS test OK\n");
            P.Dispose();
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
        /// <returns>resoult of test</returns>
        public static bool CustomTest(uint wordSize, uint testPrecision, uint primePrecision, RandomNumberGenerator generator, ulong primeDistance=uint.MaxValue)
        {
            if (wordSize < 64 || testPrecision < 1)
            {
                System.Console.WriteLine("ZKFS test invalid input\n");
                return false;
            }

            uint iteration = 0;
            bool ran;
            BigInteger com;
            bool result = true;
            KeyGen kg = new KeyGen();
            kg.KeyCreate(generator, wordSize, primeDistance, primePrecision);
            Verifier V = new Verifier(kg.PublicKey, kg.Module);
            Proover P = new Proover(kg.PrivateKey, kg.Module, generator);

            //test with key
            while (iteration < testPrecision && result)
            {
                com = P.Step1();
                ran = V.Step1(ref com);
                com = P.Step2(ran);
                V.Step2(com);
                result = V.Checkstate();
                iteration++;
            }

            if (!result) //if not verified, fail
            {
                System.Console.WriteLine("ZKFS test ERROR\n");
                return false;
            }

            //test without key
            BigInteger falseKey = kg.PrivateKey - (kg.PrivateKey / 3);
            P = new Proover(falseKey, kg.Module);
            iteration = 0;
            while (iteration < testPrecision && result)
            {
                com = P.Step1();
                ran = V.Step1(ref com);
                com = P.Step2(ran);
                V.Step2(com);
                result = V.Checkstate();
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
