//version V.2.1

using System.Numerics;

namespace ZK_Fiat_Shamir
{
    /// <summary>
    /// Utility for prime numbers.
    /// for internal use, for now.
    /// </summary>
    internal interface IPrime
    {
        /// <summary>
        /// Version to use with threads.
        /// On termination current contains next prime number.
        /// </summary>
        /// <param name="current">current number</param>
        /// <returns>next prime number</returns>
        BigInteger NextPrime(object current);

        /// <summary>
        /// Return the first prime number following the argument.
        /// </summary>
        /// <param name="number">current number</param>
        /// <returns>next prime number</returns>
        BigInteger NextPrime(BigInteger number);

        /// <summary>
        /// Primality test.
        /// </summary>
        /// <param name="number">number to test</param>
        /// <returns>true if number is prime</returns>
        bool IsPrime(ref BigInteger number);
    }
}