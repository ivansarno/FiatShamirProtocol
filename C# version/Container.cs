//version V.2.0

using System.Numerics;


namespace ZK_Fiat_Shamir
{
    internal class Container
    {
        public BigInteger Content { get; set; }

        public Container(BigInteger content)
        {
            Content = content;
        }
    }
}
