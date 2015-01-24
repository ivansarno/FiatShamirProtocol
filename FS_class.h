//
//  FS_class.h
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//

#ifndef __Fiat_Shamir__FS_class__
#define __Fiat_Shamir__FS_class__

#include "FS_aux_fun.h"

typedef struct
{
    Intero Mod;
    Intero K;
} Key;

class Verifier;
class Proover;
void FS_key_create(Key &Pubkey, Key &Privkey);

#endif /* defined(__Fiat_Shamir__FS_class__) */
