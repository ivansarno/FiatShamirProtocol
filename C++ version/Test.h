//
//  Test.hpp
//  ZK
//
//  Created by ivan sarno on 22/10/15.
//  Copyright © 2015 ivan sarno. All rights reserved.
//
//version V.3.5

#ifndef Test_hpp
#define Test_hpp

#include <stdio.h>
#include <stdint.h>
#include "Utils.h"

namespace ZKFS
{
    namespace Test
    {
        //size: number of bit of the key
        //test_precision: number of iteration, error probability = 1/2^precision
        bool DefaultTest(unsigned int size = 1024, unsigned int test_precision = 20);
        
        //version with full KeyGen parameters
        bool CustomTest(unsigned int size, unsigned int test_precision, unsigned int prime_precision,  Utils::Generator generator, unsigned int prime_distance = UINT32_MAX);
    }
}

#endif /* Test_hpp */
