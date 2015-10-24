//
//  Test.cpp
//  ZK
//
//  Created by ivan sarno on 22/10/15.
//  Copyright © 2015 ivan sarno. All rights reserved.
//
//version V.3.1

#include "Test.h"
#include "ZK-Fiat-Shamir.h"
#include <iostream>

using namespace ZKFS;
using namespace Test;
using namespace Aux;

void Test::test(unsigned int size, unsigned int precision)
{
    int iteration;
    bool ran;
    BigInteger com;
    iteration = 0;
    bool result = true;
    Aux::Generator gen;
    BigInteger priv, pub, modulus;
    KeyGen(pub, priv, modulus, gen, size);
    
    //test with key
    {
        Verifier V(pub, modulus);
        Proover P(priv, modulus, size);
        
        while(iteration < precision && result)
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
        return;
    }
    
    //test without key
    iteration = 0;
    
    {
        Verifier V1(pub, modulus);
        Proover P1(gen.get(size), modulus, size);
        
        
        while(iteration < precision && result)
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
        return;
    }
    
    std::cout << "ZKFS test OK\n";
}
