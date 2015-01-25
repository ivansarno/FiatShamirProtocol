//
//  FS_class.cpp
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//

#include "FS_class.h"


class Verifier
{
    int e;
    Key Pubkey;
    Intero u;
    bool state;
    
    Verifier(Key initkey) //take Pubkey
    {
        Pubkey.Mod = initkey.Mod;
        Pubkey.K = initkey.K;
    }
    
public:
    int step1(Intero Pu) //take result of Proover step1
    {
        srand((int)time(NULL));
        u=Pu;
        e=rand() % 2;
        return e;
    }
    
    void step2(Intero z) //take retult of Proover step2 and change the state
    {
        z= (z*z) % Pubkey.Mod;
        
        Intero y;
        
        if (e==1)
            y = (u * Pubkey.K) % Pubkey.Mod;
        else y= u;
        
        if (z==y)
            state = true;
        else state = false;
    }
    
    bool checkstate() //return state of identification
    {
        return state;
    }
    
    void reset() //reset for next iteration
    {
        e=0;
        u=0;
        state=false;
    }
    
    
};



class Proover
{
    Key Privkey;
    Intero r;
    
    Proover(Key initkey) //take Privkey
    {
        Privkey.Mod = initkey.Mod;
        Privkey.K = initkey.K;
    }
    
public:
    Intero step1() //start protocol
    {
        Randinit
        r= RandNum % Privkey.Mod;
        return (r*r) % Privkey.Mod;
    }
    
    Intero step2(int e) //take result of Verifier step1
    {
        if (e==0)
            return r;
        else if (e==1)
            return (r*Privkey.K) % Privkey.Mod;
        else return 0; 
    }
    
    void reset() //reset for next iteration
    {
       r=0;
    }
};


void FS_key_create(Key &Pubkey, Key &Privkey)
{
    Intero PrimeP=Primegen;
    Intero PrimeQ=Primegen;
    
    Randinit
    
    Privkey.Mod = PrimeP * PrimeQ;
    Pubkey.Mod = Privkey.Mod;
    
    Privkey.K = RandNum % Privkey.Mod;
    Pubkey.K = Privkey.K * Privkey.K % Privkey.Mod;
}