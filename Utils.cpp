//
//  Utils.cpp
//  Zero Knowledge Fiat-Shamir Protocol
//
//  Created by ivan sarno on 24/08/15.
//  Copyright (c) 2015 ivan sarno.
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
//version V.4.0

#include "Utils.h"

using namespace FiatShamirProtocol;
using namespace Utils;


//random number generator, only for test
Utils::TestGenerator::TestGenerator()
{
    mpz_init(rand);
    std::random_device rd;
    seed = rd();
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, seed);
}

//random number generator, only for test
Utils::TestGenerator::TestGenerator(unsigned long long seed)
{
    mpz_init(rand);
    this->seed = seed;
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, this->seed);
}

Utils::TestGenerator::~TestGenerator()
{
    mpz_clear(rand);
    gmp_randclear(rstate);
}

BigInteger Utils::TestGenerator::getBig(unsigned int size)
{
    mpz_urandomb(rand, rstate, size);
    return BigInteger(rand);
};

unsigned long long Utils::TestGenerator::getInt()
{
    BigInteger temp = this->getBig(64);
    return temp.get_si();
}


BigInteger Utils::pow(BigInteger base, BigInteger exp)
{
    BigInteger resoult = 1;
    
    while(exp > 0)
    {
        if(BigInteger(exp & 1) == 1)
            resoult *= base;
        base *= base;
        exp >>=1;
    }
    
    return resoult;
}

BigInteger Utils::mod_pow(BigInteger base, BigInteger exp, const BigInteger &mod)
{
    BigInteger resoult = 1;
    
    while(exp > 0)
    {
        if(BigInteger(exp & 1) == 1)
            resoult = (base * resoult) % mod;
        base = (base * base) % mod;
        exp >>=1;
    }
    
    return resoult;
}

bool Utils::coprime (BigInteger a, BigInteger b)
{
    if (b == 0)
        return false;
    
    BigInteger temp;
    long i = 0;
    
    while(b > 0) //find greatest common divisor
    {
        i++;
        temp = b;
        b = a % b;
        a = temp;
    }
    
    return a == 1;
}


BigInteger Utils::byte2biginteger(uint8_t *byte, unsigned int size)
{
    mpz_t z;
    mpz_init(z);
    mpz_import(z, size, 1, sizeof(byte[0]), 0, 0, byte);
    BigInteger r = BigInteger(z);
    mpz_clear(z);
    return r;
}