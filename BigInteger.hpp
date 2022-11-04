//
//  BigInteger.hpp
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
//version V.5.1

#pragma once
#include <cstdint>
#include <vector>
#include <gmpxx.h>

namespace BigInt
{
    typedef mpz_class BigInteger;
    typedef std::vector<uint8_t> Buffer;
    BigInteger pow(const BigInteger &base, uint64_t exp);
    BigInteger modPow(const BigInteger &base, const BigInteger &exp, const BigInteger &mod);
    BigInteger&& pow(BigInteger &&base, uint64_t exp);
    BigInteger&& modPow(BigInteger &&base, const BigInteger &exp, const BigInteger &mod);
    bool coprime (const BigInteger &a, const BigInteger &b);
    BigInteger inverse(const BigInteger &number, const BigInteger &mod);
    BigInteger&& inverse(BigInteger &&number, const BigInteger &mod);
    bool isPrime(const BigInteger &number, unsigned iterations = 20);
    BigInteger nextPrime(const BigInteger &number);
    //retuns -1 for error
    BigInteger bigIntegerFromBytes(const Buffer &data);
    Buffer bigIntegerToBytes(const BigInteger &number);
    //retuns -1 for error
    BigInteger bigIntegerFromBytes(const Buffer &data, size_t &index);
    void bigIntegerToBytes(const BigInteger &number, Buffer &data);
    bool isOdd(const BigInteger &number);
    bool isEven(const BigInteger &number);
    bool isNegative(const BigInteger &number);
    bool isPositive(const BigInteger &number);
    bool isZero(const BigInteger &number);
    unsigned bit(const BigInteger &number, size_t index);
    BigInteger mod(const BigInteger &number, const BigInteger &modulus);
    BigInteger&& mod(BigInteger &&number, const BigInteger &modulus);
    BigInteger negMod(const BigInteger &number, const BigInteger &modulus);
    BigInteger&& negMod(BigInteger &&number, const BigInteger &modulus);
    BigInteger u64ToBigInteger(uint64_t integer);
    uint64_t bigIntegerToU64(const BigInteger &big);
    unsigned bitSize(const BigInteger &number);
    
    BigInteger powModSec(const BigInteger &base, const BigInteger &exp, const BigInteger &mod);
    
    void integersToBytes(const std::vector<BigInteger> &integers, Buffer &data, unsigned intSize=0);
    Buffer integersToBytes(const std::vector<BigInteger> &integers, unsigned intSize=0);
    //returns an empty vector for error or if data is empty
    std::vector<BigInteger> integersFromBytes(const Buffer &data, size_t &index);
    //returns an empty vector for error or if data is empty
    std::vector<BigInteger> integersFromBytes(const Buffer &data);
    
    void erase(BigInteger &secret);
}

