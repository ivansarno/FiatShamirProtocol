//
//  ZK-Fiat-Shamir.cpp
//  Zero Knowledge Fiat-Shamir Protocol
//
//  Created by ivan sarno on 24/08/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
//version V.3.6

#ifndef __ZK__Prime__
#define __ZK__Prime__
#include <gmpxx.h>
#include "Utils.h"


namespace ZKFS
{
    namespace Prime
    {
        using namespace Utils;
        
        //gen = random number generator
        //size = number of  bit of namber tested
        //precision = error of MIller-Rabin test = 1/2^(2* precision)
        //seed = seed for random number generator
        bool IsPrime(const BigInteger &number, unsigned long long seed, unsigned int size, unsigned int precision = 20);
        
        //gen = random number generator
        //size = number of  bit of prime generated
        //precision = error of MIller-Rabin test = 1/2^(2* precision)
        //seed = seed for random number generator
        BigInteger NextPrime(BigInteger current, unsigned long long seed, unsigned int size, unsigned int precision = 20);
    }
}

#endif /* defined(__ZK__Prime__) */
