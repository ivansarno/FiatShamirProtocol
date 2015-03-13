//
//  FS_class.h
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
// version V.1.5

#ifndef __Fiat_Shamir__FS_class__
#define __Fiat_Shamir__FS_class__

#include "FS_aux_fun.h"


typedef struct
{
    Intero Mod;
    Intero K; //key
} Key;


void FS_key_create(Key &Pubkey, Key &Privkey); //take uninitialized Key variable and generates Public and Private key



class Proover
{
    Key Privkey;
    Intero r;
    
public:
    Proover(Key initkey); //take Privkey
    Intero step1(); //start protocol
    Intero step2(int e); //take result of Verifier step1
    void reset(); //reset the object (to another session)
};


class Verifier
{
    int e;
    Key Pubkey;
    Intero u;
    bool state;
    
public:
    Verifier(Key initkey); //take Pubkey
    int step1(Intero Pu); //take result of Proover step1
    void step2(Intero z); //take retult of Proover step2 and change the state
    bool checkstate(); //return state of identification
    void reset(); //reset reset the object (to another session)
};

//subfunctions
bool Prime_check(Intero Q, Intero P); //test for prime number security

#endif /* defined(__Fiat_Shamir__FS_class__) */
