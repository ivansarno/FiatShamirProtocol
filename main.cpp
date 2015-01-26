//
//  main.cpp
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//

#include <iostream>
#include "FS_class.h"

int main(int argc, const char * argv[])
{
    Key Privkey, Pubkey;
    
    FS_key_create(Pubkey, Privkey);
    
   // Privkey.K="98472394872394";

    
    Proover P(Privkey);
    Verifier V(Pubkey);
    
    
    
    int i,e;
    Intero t;
    i=0;
    bool s=true;
    srand((int)time(NULL));
    
    while (i<20 && s)
    {
        t=P.step1();
        e=V.step1(t);
        t=P.step2(e);
        V.step2(t);
        i++;
        s=V.checkstate();
    }
    
    
    
    
    
    
    std::cout <<  i << "\n" <<  s << "\n";
    return 0;
}
