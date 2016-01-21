//
//  FiatShamirProtocol.cpp
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

#include "FiatShamirProtocol.h"

using namespace FiatShamirProtocol;




Proover::Proover(BigInteger &privkey, BigInteger &modulus, unsigned int size, Utils::Generator *generator)
{
    key = privkey;
    this->modulus = modulus;
    this->size = size;
    gen = generator;
}

BigInteger Proover::step1()
{
    session_number = gen->getBig(size) % modulus;
    
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

Verifier::Verifier(BigInteger &pubkey, BigInteger &modulus)
{
    key = pubkey;
    this->modulus = modulus;
    state = false;
    srand((unsigned int)time(NULL));//init generator
}

bool Verifier::step1(BigInteger &session_number) //take result of Proover step1
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
inline bool prime_check(const BigInteger &Q, const BigInteger &P, unsigned long distance)
{
    BigInteger dif = abs(P-Q);
    return dif > distance;
}

bool FiatShamirProtocol::KeyGen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, Utils::Generator *gen, unsigned int size, unsigned int precision, unsigned long distance)
{
    if(size < 64 || precision < 1)
        return false;
    
    
    BigInteger primeP = Prime::NextPrime(gen->getBig(size/2), size/2, precision);
    BigInteger primeQ = Prime::NextPrime(gen->getBig(size/2), size/2, precision);
    
    
    while(!prime_check(primeP, primeQ, distance))
    {
        primeQ = Prime::NextPrime(gen->getBig(size/2), size/2, precision);
    }
    
    
    modulus = primeP * primeQ;
    privkey = gen->getBig(size) % modulus;
    pubkey = (privkey * privkey) % modulus;
    
    return true;
}

//prime extraction routine for 2 threads
inline void DualRoutine(BigInteger &primeP, BigInteger &primeQ, Utils::Generator *gen, unsigned int size, unsigned int precision, unsigned long distance)
{
    primeP = gen->getBig(size/2);
    auto worker = std::thread(TNextPrime, &primeP, size/2, precision);
    primeQ = Prime::NextPrime(gen->getBig(size/2), size/2, precision);
    worker.join();
    
    
    while(!prime_check(primeP, primeQ, distance))
    {
        primeQ = gen->getBig(size/2);
        Prime::PNextPrime(&primeQ, size/2, precision);
    }
}

//multithread prime extraction routine
inline void ParallelRoutine(BigInteger &primeP, BigInteger &primeQ, Utils::Generator *gen, unsigned int size, unsigned int precision, unsigned long distance, int threads)
{
    primeP = gen->getBig(size/2);
    auto worker = std::thread(PNextPrime, &primeP, size/2, precision, threads/2);
    primeQ = gen->getBig(size/2);
    Prime::PNextPrime(&primeQ, size/2, precision, (threads-threads/2));
    worker.join();
    
    
    while(!prime_check(primeP, primeQ, distance))
    {
        primeQ = gen->getBig(size/2);
        Prime::PNextPrime(&primeQ, size/2, precision, threads);
    }
}

bool FiatShamirProtocol::ParallelKeyGen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, Utils::Generator *gen, unsigned int size, int threads, unsigned int precision, unsigned long distance)
{
    if(threads < 2)
        return KeyGen(pubkey, privkey, modulus, gen, size, precision, distance);
    if(size < 64 || precision < 2)
        return false;
    
    BigInteger primeP, primeQ;
    
    if(threads < 4)
        DualRoutine(primeP, primeQ, gen, size, precision, distance);
    else ParallelRoutine(primeP, primeQ, gen, size, precision, distance, threads);
    
    
    modulus = primeP * primeQ;
    privkey = gen->getBig(size) % modulus;
    pubkey = (privkey * privkey) % modulus;
    
    return true;
}
