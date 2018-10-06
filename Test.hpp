//
//  Test.h
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



#pragma once

#include "FiatShamirProtocol.hpp"
#include <random>

namespace FiatShamirProtocol
{
    class TestGenerator: public Generator
    {
    private:
        gmp_randstate_t rstate;
        mpz_t rand;
        unsigned long long seed;
        std::random_device rd;
    public:
        TestGenerator();

        explicit TestGenerator(unsigned long long seed);
        ~TestGenerator();
        BigInteger getBig(unsigned int size) override;

        bool getBit() override;
    };

    //size: number of bit of the key
    //testPrecision: number of iteration, error probability = 1/2^precision
    bool test(unsigned int size = 1024, unsigned int testPrecision = 20);

    bool test(unsigned int size, unsigned int testPrecision, Generator &gen);
}
