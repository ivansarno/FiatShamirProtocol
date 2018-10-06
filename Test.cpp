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
//version V.5.0


#include "Test.hpp"
#include "FiatShamirProtocol.hpp"
#include <iostream>
#include <random>

using namespace FiatShamirProtocol;


//random number generator, only for test
FiatShamirProtocol::TestGenerator::TestGenerator()
{
    mpz_init(rand);
    seed = rd();
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, seed);
}

//random number generator, only for test
FiatShamirProtocol::TestGenerator::TestGenerator(unsigned long long seed)
{
    mpz_init(rand);
    this->seed = seed;
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, this->seed);
}

FiatShamirProtocol::TestGenerator::~TestGenerator()
{
    mpz_clear(rand);
    gmp_randclear(rstate);
}

BigInteger FiatShamirProtocol::TestGenerator::getBig(unsigned int size)
{
    mpz_urandomb(rand, rstate, size);
    return BigInteger(rand);
}

bool TestGenerator::getBit()
{
    return (rd() % 2) == 0;
};

bool FiatShamirProtocol::test(unsigned int size, unsigned int testPrecision)
{
    if(size < 512 || testPrecision < 2)
    {
        std::cout <<  "FiatShamirProtocol test invalid input\n";
        return false;
    }

    auto gen = TestGenerator();
    auto priv = PrivateKey::keyGen(gen, size);
    auto pub = priv.getPublicKey();
    auto prover = priv.getProover(gen);
    auto verifier = pub.getVerifier(gen);
    BigInteger sn = 0;
    for(unsigned i = 0; i<testPrecision; i++)
    {
        sn = prover.step1();
        bool choice = verifier.step1(sn);
        sn = prover.step2(choice);
        if(!verifier.step2(sn))
        {
            std::cout <<  "FiatShamirProtocol test ERROR\n" ;
            return false;
        }
    }

    auto fakeKey = PrivateKey::keyGen(gen, size);
    if(fakeKey == priv)
    {
        auto fakeProover = fakeKey.getProover(gen);
        sn = fakeProover.step1();
        bool choice = verifier.step1(sn);
        sn = fakeProover.step2(choice);
        if(verifier.step1(sn))
        {
            std::cout <<  "FiatShamirProtocol test ERROR\n" ;
            return false;
        }
    }
    

    std::cout << "FiatShamirProtocol test OK\n";
    return true;
}

bool FiatShamirProtocol::test(unsigned int size, unsigned int testPrecision, Generator &gen)
{
    if(size < 512 || testPrecision < 2)
    {
        std::cout <<  "FiatShamirProtocol test invalid input\n";
        return false;
    }

    auto priv = PrivateKey::keyGen(gen, size);
    auto pub = priv.getPublicKey();
    auto prover = priv.getProover(gen);
    auto verifier = pub.getVerifier(gen);
    BigInteger sn;

    for(unsigned i = 0; i<testPrecision; i++)
    {
        sn = prover.step1();
        bool choice = verifier.step1(sn);
        sn = prover.step2(choice);
        if(!verifier.step1(sn))
        {
            std::cout <<  "FiatShamirProtocol test ERROR\n" ;
            return false;
        }
    }

    auto fakeKey = PrivateKey::keyGen(gen, size);
    if(fakeKey == priv)
    {
        auto fakeProover = fakeKey.getProover(gen);
        sn = fakeProover.step1();
        bool choice = verifier.step1(sn);
        sn = fakeProover.step2(choice);
        if(verifier.step1(sn))
        {
            std::cout <<  "FiatShamirProtocol test ERROR\n" ;
            return false;
        }
    }

    std::cout << "FiatShamirProtocol test OK\n";
    return true;
}
