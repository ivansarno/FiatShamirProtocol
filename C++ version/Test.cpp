//
//  Test.cpp
//  ZK
//
//  Created by ivan sarno on 22/10/15.
//  Copyright © 2015 ivan sarno. All rights reserved.
//
//version V.3.5

#include "Test.h"
#include "ZK-Fiat-Shamir.h"
#include <iostream>

using namespace ZKFS;
using namespace Test;
using namespace Utils;

//size: number of bit of the key
//test_precision: number of iteration, error probability = 1/2^precision
bool Test::DefaultTest(unsigned int size, unsigned int test_precision)
{
    if(size < 64 || test_precision < 1)
    {
        std::cout <<  "ZKFS test invalid input\n";
        return false;
    }
    
    int iteration = 0;
    bool ran;
    BigInteger com;
    bool result = true;
    Utils::Generator gen;
    BigInteger priv, pub, modulus;
    KeyGen(pub, priv, modulus, gen, size);
    
    //test with key
    {
        Verifier V(pub, modulus);
        Proover P(priv, modulus, size);
        
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
        std::cout <<  "ZKFS test ERROR\n";
        return false;
    }
    
    //test without key
    iteration = 0;
    
    {
        Verifier V1(pub, modulus);
        Proover P1(gen.get(size), modulus, size);
        
        
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
        std::cout <<  "ZKFS test ERROR\n" ;
        return false;
    }
    
    std::cout << "ZKFS test OK\n";
    return true;
}

//version with full KeyGen parameters
bool Test::CustomTest(unsigned int size, unsigned int test_precision, unsigned int prime_precision,  Generator generator, unsigned int prime_distance)
{
    if(size < 64 || test_precision < 1)
    {
        std::cout <<  "ZKFS test invalid input\n";
        return false;
    }
    
    int iteration;
    bool ran;
    BigInteger com;
    iteration = 0;
    bool result = true;
    BigInteger priv, pub, modulus;
    KeyGen(pub, priv, modulus, generator, size, prime_precision, prime_distance);
    
    //test with key
    {
        Verifier V(pub, modulus);
        Proover P(priv, modulus, size);
        
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
        std::cout <<  "ZKFS test ERROR\n";
        return false;
    }
    
    //test without key
    iteration = 0;
    
    {
        Verifier V1(pub, modulus);
        Proover P1(generator.get(size), modulus, size);
        
        
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
        std::cout <<  "ZKFS test ERROR\n" ;
        return false;
    }
    
    std::cout << "ZKFS test OK\n";
    return true;
}