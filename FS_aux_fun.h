//
//  FS_aux_fun.h
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//

#ifndef __Fiat_Shamir__FS_aux_fun__
#define __Fiat_Shamir__FS_aux_fun__

#include "Define.h"

Intero fastexp( Intero base, Intero esp);
Intero modexp( Intero base, Intero esp, Intero mod);
bool MRpredicate1 (Intero y, Intero z, Intero N);
bool MRpredicate2(Intero y, Intero N, Intero z, Intero w);
bool MRtest(Intero N); //Miller-Rabin test for prime number
void MRscomposition(Intero N, Intero &w, Intero &z);
Intero Primegenerator();
Intero inverso(Intero n, Intero Modulo);
bool coprime (Intero a,  Intero b);

#endif /* defined(__Fiat_Shamir__FS_aux_fun__) */
