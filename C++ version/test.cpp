//
//  main.cpp
//  ZK
//
//  Created by ivan sarno on 30/03/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//

#include <iostream>
#include "ZK-Fiat-Shamir.h"
using namespace ZKFS;

int main(int argc, const char * argv[]) {
    
    int iteration, result, ran;
    BigInteger com;
    iteration = 0;
    result = 1;
    Aux::Generator gen;
    BigInteger priv, pub, modulus;
    KeyGen(pub, priv, modulus, gen, 2048);
    Verifier V(pub, modulus);
    Proover P(priv, modulus, 2048);
    
    
    while(iteration < 20 && result)
    {
        com = P.step1();
        ran = V.step1(com);
        com = P.step2(ran);
        V.step2(com);
        result = V.checkstate();
        iteration++;
    }
    
    
    std::cout << result << "\n" << iteration << "\n";
    return 0;
}
