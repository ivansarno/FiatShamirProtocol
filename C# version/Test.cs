using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace ZK_Fiat_Shamir
{
    class Test
    {
        static void Main(string[] args)
        {
            KeyGen kg = new KeyGen();
            kg.KeyCreate();

            Verifier V = new Verifier( kg.PubKey,  kg.Module);
            Proover P = new Proover(kg.PrivKey,  (kg.Module));

            bool m1 = false;
            BigInteger m2 = 0;

            int i = 0;
            while (i < 20)
            {
                m2 = P.Step1();
                m1 = V.Step1(ref m2);
                m2 = P.Step2(m1);
                m1 = V.Step2(m2);

                if (!m1)
                    break;
                i++;
            }

            System.Console.WriteLine(m1 + " " + i);
        }
    }
}
