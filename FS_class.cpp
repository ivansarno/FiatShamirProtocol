//
//  FS_class.cpp
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
// version V.1.5

#include "FS_class.h"

    
Verifier::Verifier(Key initkey) //take Pubkey
{
    Pubkey.Mod = initkey.Mod;
    Pubkey.K = initkey.K;
    Randbitinit
}
    
int Verifier::step1(Intero Pu) //take result of Proover step1
{
    u=Pu;
    e=Randbit
    return e;
}
    
void Verifier::step2(Intero z) //take retult of Proover step2 and change the state
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
    
bool Verifier::checkstate() //return state of identification
{
    return state;
}
    
void Verifier::reset() //reset for next session
{
    e=0;
    u=0;
    state=false;
    Randbitinit
}

    
Proover::Proover(Key initkey) //take Privkey
{
    Privkey.Mod = initkey.Mod;
    Privkey.K = initkey.K;
    Randinit
}
    

Intero Proover::step1() //start protocol
{
    r= RandNum;
    r %= Privkey.Mod;
    return (r*r) % Privkey.Mod;
}
    
Intero Proover::step2(int e) //take result of Verifier step1
{
    if (e==0)
        return r;
    else if (e==1)
        return (r*Privkey.K) % Privkey.Mod;
    else return 0;
}
    
void Proover::reset() //reset for next session
{
    r=0;
    Randinit
        
}



void FS_key_create(Key &Pubkey, Key &Privkey) //take uninitialized K variable and generates Public and Private key
{
    Randinit
    Intero PrimeP=Primegen;
    Intero PrimeQ=Primegen;
    
    while (Prime_check(PrimeQ, PrimeP)) //make sure it is appropriate for security standards
    {
        PrimeQ = Primegen;
    }
    
    
    Privkey.Mod = PrimeP * PrimeQ;
    Pubkey.Mod = Privkey.Mod;
    
    Privkey.K = RandNum;
    Privkey.K %= Privkey.Mod;
    Pubkey.K = Privkey.K * Privkey.K % Privkey.Mod;
}

bool Prime_check(Intero Q, Intero P) //test for prime number security
{
    Intero dif = (P-Q);
    dif.Abs();
    P=(P-1)/2;
    Q=(Q-1)/2;
    
    return coprime(P,Q) && (dif > Distance);
}