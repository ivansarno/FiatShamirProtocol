//
//  Prime.h
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
//version V.4.1

#pragma once
#include <gmpxx.h>
#include "Utils.h"
#include <thread>


namespace FiatShamirProtocol
{
    namespace Prime
    {
        using namespace Utils;
        
        //gen = random number generator
        //size = number of  bit of namber tested
        //precision = error of MIller-Rabin test = 1/2^(2* precision)
        //seed = seed for random number generator
        bool IsPrime(const BigInteger &number, unsigned int size, unsigned int precision = 20);
        
        //gen = random number generator
        //size = number of  bit of prime generated
        //precision = error of Miller-Rabin test = 1/2^(2* precision)
        //seed = seed for random number generator
        BigInteger NextPrime(BigInteger current, unsigned int size, unsigned int precision = 20);
        
        //to use with threads
        //gen = random number generator
        //size = number of  bit of prime generated
        //precision = error of Miller-Rabin test = 1/2^(2* precision)
        //seed = seed for random number generator
        //current contains the next prime
        void ThreadsNextPrime(BigInteger *current, unsigned int size, unsigned int precision = 20);
        
        //multithread version of ThreadsNextPrime
        //gen = random number generator
        //size = number of  bit of prime generated
        //precision = error of Miller-Rabin test = 1/2^(2* precision)
        //seed = seed for random number generator
        //threads = number of threads to use
        //current contains the next prime
        void ParallelNextPrime(BigInteger *current, unsigned int size, unsigned int precision = 20, int threads = 2);

    }
}
