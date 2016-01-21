//
//  FiatShamirProtocol.h
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

#pragma once
#include <stdio.h>
#include "Prime.h"
#include <thread>

namespace FiatShamirProtocol
{
    using namespace Utils;
    using namespace Prime;
    
    class Proover
    {
    private:
        BigInteger key;
        BigInteger modulus;
        BigInteger session_number;
        Utils::Generator *gen;
        int size;
        
    public:
        //size = number of bit of modulus
        //generator = random number generator, use an your own implementation of abstract class Generator or my TestGenerator only for test
        Proover(BigInteger &privkey, BigInteger &modulus, unsigned int size, Utils::Generator *generator);// subclass of my class for random number generator
        
        //Start the protocol and return the session number to send to Verifier
        BigInteger step1();
        
        //Take result of verifier step1, a random bool, ad return the proof to send to Verifier
        BigInteger step2(bool choice);
        
    };
    
    class Verifier
    {
    private:
        bool choice;
        BigInteger key;
        BigInteger modulus;
        BigInteger session_number;
        bool state;
        
    public:
        Verifier(BigInteger &pubkey, BigInteger &modulus);
        
        //take result of Proover step1 and return a random choice to send to Proover
        bool step1(BigInteger &session_number);
        
        //take retult of Proover step2, change the state of identification and return this
        bool step2(BigInteger proof);
        
        //return state of identification
        bool checkstate();
    };
    
    //initializes the keys and modulus,
    //size = number of bit of the keys,
    //distance is distance between prime number of the key, used for security check
    //precision is precision of Miller-Rabin primality test, error is 1/2^2*precision
    //gen = random number generator, use an your own implementation of abstract class Generator or my TestGenerator only for test
    bool KeyGen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, Utils::Generator *gen, unsigned int size, unsigned int precision = 20, unsigned long distance = UINT32_MAX);
    
    //multithread version
    //size = number of bit of the keys,
    //distance is distance between prime number of the key, used for security check
    //precision is precision of Miller-Rabin primality test, error is 1/2^2*precision
    //gen = random number generator, use an your own implementation of abstract class Generator or my TestGenerator only for test
    //threads = number of threads to use
    bool ParallelKeyGen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, Utils::Generator *gen, unsigned int size, int threads, unsigned int precision = 20, unsigned long distance = UINT32_MAX);
}
