//
//  Test.cpp
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


#include "Test.h"
#include "FiatShamirProtocol.h"
#include <iostream>

using namespace FiatShamirProtocol;
using namespace Test;
using namespace Utils;

//size: number of bit of the key
//test_precision: number of iteration, error probability = 1/2^precision
bool Test::DefaultTest(unsigned int size, unsigned int test_precision)
{
    if(size < 64 || test_precision < 1)
    {
        std::cout <<  "FiatShamirProtocol test invalid input\n";
        return false;
    }
    
    int iteration = 0;
    bool ran;
    BigInteger com;
    bool result = true;
    Utils::TestGenerator gen;
    BigInteger priv, pub, modulus;
    KeyGen(pub, priv, modulus, &gen, size);
    
    //test with key
    {
        Verifier V(pub, modulus);
        Proover P(priv, modulus, size, &gen);
        
        while(iteration < test_precision && result)
        {
            com = P.step1();
            ran = V.step1(com);
            com = P.step2(ran);
            V.step2(com);
            result = V.checkstate();
            iteration++;
        }
    }
    
    if(!result) //if not verified, fail
    {
        std::cout <<  "FiatShamirProtocol test ERROR\n";
        return false;
    }
    
    //test without key
    iteration = 0;
    
    {
        Verifier V1(pub, modulus);
        BigInteger false_key = priv-(priv/3);
        Proover P1(false_key, modulus, size, &gen);
        
        
        while(iteration < test_precision && result)
        {
            com = P1.step1();
            ran = V1.step1(com);
            com = P1.step2(ran);
            V1.step2(com);
            result = V1.checkstate();
            iteration++;
        }
    }
    
    if(result) //if verified, fail
    {
        std::cout <<  "FiatShamirProtocol test ERROR\n" ;
        return false;
    }
    
    std::cout << "FiatShamirProtocol test OK\n";
    return true;
}

//version with full KeyGen parameters
bool Test::CustomTest(unsigned int size, unsigned int test_precision, unsigned int prime_precision,  Generator *generator, int threads, unsigned int prime_distance)
{
    if(size < 64 || test_precision < 1)
    {
        std::cout <<  "FiatShamirProtocol test invalid input\n";
        return false;
    }
    
    int iteration;
    bool ran;
    BigInteger com;
    iteration = 0;
    bool result = true;
    BigInteger priv, pub, modulus;
    ParallelKeyGen(pub, priv, modulus, generator, size, threads, prime_precision, prime_distance);
    
    //test with key
    {
        Verifier V(pub, modulus);
        Proover P(priv, modulus, size, generator);
        
        while(iteration < test_precision && result)
        {
            com = P.step1();
            ran = V.step1(com);
            com = P.step2(ran);
            V.step2(com);
            result = V.checkstate();
            iteration++;
        }
    }
    
    if(!result) //if not verified, fail
    {
        std::cout <<  "FiatShamirProtocol test ERROR\n";
        return false;
    }
    
    //test without key
    iteration = 0;
    
    {
        Verifier V1(pub, modulus);
        BigInteger false_key = priv-(priv/3);
        Proover P1(false_key, modulus, size, generator);
        
        
        while(iteration < test_precision && result)
        {
            com = P1.step1();
            ran = V1.step1(com);
            com = P1.step2(ran);
            V1.step2(com);
            result = V1.checkstate();
            iteration++;
        }
    }
    
    if(result) //if verified, fail
    {
        std::cout <<  "FiatShamirProtocol test ERROR\n" ;
        return false;
    }
    
    std::cout << "FiatShamirProtocol test OK\n";
    return true;
}