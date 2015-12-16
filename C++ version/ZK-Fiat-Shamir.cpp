//
//  ZK-Fiat-Shamir.cpp
//  Zero Knoledge Fiat-Shamir Protocol
//
//  Created by ivan sarno on 24/08/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
//version V.3.5

#include "ZK-Fiat-Shamir.h"

using namespace ZKFS;

Proover::Proover(BigInteger privkey, BigInteger modulus, unsigned int size)
{
    key = privkey;
    this->modulus = modulus;
    this->size = size;
    gen = Utils::Generator();
}

Proover::Proover(BigInteger privkey, BigInteger modulus, unsigned int size, Utils::Generator generator)
{
    key = privkey;
    this->modulus = modulus;
    this->size = size;
    gen = generator;
}

BigInteger Proover::step1()
{
    session_number = gen.get(size) % modulus;

    while(session_number < 2)//avoid comunication of the key
       session_number = (session_number + 2) % modulus;

    return (session_number * session_number) % modulus;
}

BigInteger Proover::step2(bool choice)
{
    if(choice)
        return (session_number * key) % modulus;
    return session_number;
}

Verifier::Verifier(BigInteger pubkey, BigInteger modulus)
{
    key = pubkey;
    this->modulus = modulus;
    state = false;
    srand((unsigned int)time(NULL));//init generator
}

bool Verifier::step1(BigInteger session_number) //take result of Proover step1
{
    if(session_number == 0) //avoid attack
        this->session_number=1;
    this->session_number=session_number;
    choice=(rand() % 2) == 1;
    return choice;
}

bool Verifier::step2(BigInteger proof) //take retult of Proover step2 and change the state
{
    proof = (proof*proof) % modulus;

    BigInteger y;

    if (choice)
        y = (session_number * key) % modulus;
    else y= session_number;

    state = proof==y;

    return state;
}

bool Verifier::checkstate() //return state of identification
{
    return state;
}

//check security standard conformance
bool prime_check(BigInteger Q, BigInteger P, unsigned long distance)
{
    BigInteger dif = (P-Q);

    return abs(dif) > distance;
}

bool ZKFS::KeyGen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, Utils::Generator gen, unsigned int size, unsigned int precision, unsigned long distance)
{
    if(size < 64 || precision < 1)
        return false;


    BigInteger primeP = Prime::Generates(gen, size/2, precision);
    BigInteger primeQ = Prime::Generates(gen, size/2, precision);

#ifdef ZK_check
    while(!prime_check(primeP, primeQ, distance))
    {
        primeQ = Prime::Generates(gen, size /2, precision);
    }
#endif

    modulus = primeP * primeQ;
    privkey = gen.get(size) % modulus;
    pubkey = (privkey * privkey) % modulus;

    return true;
}
