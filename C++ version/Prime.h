//
//  ZK-Fiat-Shamir.cpp
//  Zero Knoledge Fiat-Shamir Protocol
//
//  Created by ivan sarno on 24/08/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
//version V.3.0

#ifndef __RSA__Prime__
#define __RSA__Prime__
#include <gmpxx.h>
#include "Aux.h"


namespace ZKFS
{
    namespace Prime
    {
        using namespace Aux;
        
        bool IsPrime(BigInteger number, Generator gen, int size, int precision = 20); //precision of Miller-Rabin test, error = 1/2^2n
        BigInteger Generates(Generator gen, int size, int precision = 20);
    }
}

#endif /* defined(__RSA__Prime__) */
