//
//  ZK-Fiat-Shamir.cpp
//  Zero Knoledge Fiat-Shamir Protocol
//
//  Created by ivan sarno on 24/08/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
//version V.3.0

#ifndef __ZK__ZK_Fiat_Shamir__
#define __ZK__ZK_Fiat_Shamir__

#include <stdio.h>
#include "Prime.h"

namespace ZKFS
{
    using namespace Aux;
    using namespace Prime;
    
    class Proover
    {
    private:
        BigInteger key;
        BigInteger modulus;
        BigInteger session_number;
        Aux::Generator gen;
        int size;
        
    public:
        Proover(BigInteger privkey, BigInteger modulus, int size);// size = number of bit of the key
        Proover(BigInteger privkey, BigInteger modulus, int size, Aux::Generator generator);
        BigInteger step1();
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
        Verifier(BigInteger pubkey, BigInteger modulus);
        bool step1(BigInteger session_number); //take result of Proover step1 and return a random choice
        bool step2(BigInteger proof); //take retult of Proover step2, change the state and return this
        bool checkstate(); //return state of identification
    };
    
    //initializes the keys and modulus, size = number of bit of the keys, distance is distance between prime number of the key, precision is precision of Miller-Rabin primality test
    void KeyGen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, Aux::Generator gen, int size, int precision = 20, unsigned long = UINT32_MAX);
}

#endif /* defined(__ZK__ZK_Fiat_Shamir__) */
