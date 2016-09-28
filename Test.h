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
//version V.4.1



#pragma once
#include <stdio.h>
#include <stdint.h>
#include "Utils.h"

namespace FiatShamirProtocol
{
    namespace Test
    {
        //size: number of bit of the key
        //test_precision: number of iteration, error probability = 1/2^precision
        bool DefaultTest(unsigned int size = 1024, unsigned int test_precision = 20);
        
        //version with full KeyGen parameters
        bool CustomTest(unsigned int size, unsigned int test_precision, unsigned int prime_precision,  Utils::Generator *generator, int threads = 1, unsigned int prime_distance = UINT32_MAX);
    }
}
