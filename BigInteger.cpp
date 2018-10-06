//
//  BigInteger.cpp
//  FiatShamirProtocol
//
//  Created by ivan sarno on 04/10/2018.
//  Copyright © 2018 ivan sarno.
//
/*
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
//version V.5.0

#include "BigInteger.hpp"

using namespace BigInt;
using namespace std;

BigInteger BigInt::pow(const BigInteger &base, unsigned long exp)
{
    
    BigInteger result = BigInteger();
    mpz_pow_ui(result.__get_mp(), base.get_mpz_t(), exp);
    return result;
}
BigInteger BigInt::modPow(const BigInteger &base, const BigInteger &exp, const BigInteger &mod)
{
    BigInteger result = BigInteger();
    mpz_powm(result.__get_mp(), base.__get_mp(), exp.get_mpz_t(), mod.__get_mp());
    return result;
}
bool BigInt::coprime(const BigInteger &a, const BigInteger &b)
{
    return gcd(a,b) == 1;
}
BigInteger BigInt::inverse(const BigInteger &number, const BigInteger &mod)
{
    BigInteger result = BigInteger();
    mpz_invert(result.get_mpz_t(), number.get_mpz_t(), mod.get_mpz_t());
    return result;
    
}
bool BigInt::isPrime(const BigInteger &number)
{
   return 0 != mpz_probab_prime_p(number.get_mpz_t(), 20);
}
BigInteger BigInt::nextPrime(const BigInteger &number)
{
    BigInteger r;
    mpz_nextprime(r.get_mpz_t(), number.get_mpz_t());
    return r;
}
//retuns -1 for error
BigInteger BigInt::bigIntegerFromBytes(const Buffer &data)
{
    size_t index = 0;
    return bigIntegerFromBytes(data, index);
}
Buffer BigInt::bigIntegerToBytes(const BigInteger &number)
{
    auto data = Buffer();
    bigIntegerToBytes(number, data);
    return data;
}
//retuns -1 for error
BigInteger BigInt::bigIntegerFromBytes(const Buffer &data, size_t &index)
{
    if(data.empty() || data.size() < index || data.size() < data[index] + index || data[index] == 0)
    {
        index = data.size();
        return -1;
    }
    BigInteger r;
    mpz_import(r.get_mpz_t(), data[index], 1, sizeof(uint8_t), 1, 0, data.data()+index+1);
    index += data[index]+1;
    return r;
}
void BigInt::bigIntegerToBytes(const BigInteger &number, Buffer &data)
{
    size_t size = (mpz_sizeinbase(number.get_mpz_t(), 2) + 8-1) / 8;
    size_t index = data.size();
    data.resize(data.size() + size + 1, 0);
    data[index] = (uint8_t) size;
    index++;
    mpz_export(data.data()+index, nullptr, 1, sizeof(uint8_t), 1, 0, number.get_mpz_t());
}
bool BigInt::isOdd(const BigInteger &number)
{
    return mpz_even_p(number.get_mpz_t()) == 0;
}
bool BigInt::isEven(const BigInteger &number)
{
    return mpz_odd_p(number.get_mpz_t()) == 0;
}
bool BigInt::isNegative(const BigInteger &number)
{
    return mpz_sgn(number.get_mpz_t()) == -1;
}
bool BigInt::isPositive(const BigInteger &number)
{
    return mpz_sgn(number.get_mpz_t()) == 1;
}
bool BigInt::isZero(const BigInteger &number)
{
    return mpz_sgn(number.get_mpz_t()) == 0;
}
unsigned BigInt::bit(const BigInteger &number, size_t index)
{
    return static_cast<unsigned int>(mpz_tstbit(number.get_mpz_t(), index));
}
BigInteger BigInt::mod(const BigInteger &number, const BigInteger &modulus)
{
    BigInteger res = BigInteger();
    mpz_mod(res.get_mpz_t(), number.get_mpz_t(), modulus.get_mpz_t());
    return res;
}
BigInteger BigInt::u64ToBigInteger(uint64_t integer)
{
    BigInteger big = 0;
    mpz_import(big.get_mpz_t(), 1, -1, sizeof(integer), 0, 0, &integer);
    return big;
}
uint64_t BigInt::bigIntegerToU64(const BigInteger &big)
{
    uint64_t result = 0;
    mpz_export(&result, nullptr, -1, sizeof(result), 0, 0, big.__get_mp());
    return result;
}

BigInteger BigInt::powModSec(const BigInteger &base, const BigInteger &exp, const BigInteger &mod)
{
    BigInteger result = BigInteger();
    mpz_powm_sec(result.__get_mp(), base.__get_mp(), exp.get_mpz_t(), mod.__get_mp());
    return result;
}

unsigned BigInt::bitSize(const BigInteger &number)
{
    return static_cast<unsigned int>(mpz_sizeinbase(number.get_mpz_t(), 2));
}

