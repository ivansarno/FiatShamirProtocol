//
//  FS_aux_fun.h
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
// version V.2.0

#ifndef __Fiat_Shamir__FS_aux_fun__
#define __Fiat_Shamir__FS_aux_fun__

#include "Define.h"

Integer fastexp( Integer base, Integer esp);
Integer modexp( Integer base, Integer esp, Integer mod); //power in module
bool MRpredicate1 (Integer y, Integer z, Integer N);
bool MRpredicate2(Integer y, Integer N, Integer z, Integer w);
bool MRtest(Integer N); //Miller-Rabin test for prime number
void MRscomposition(Integer N, Integer &w, Integer &z);
Integer Primegenerator();
Integer inverso(Integer n, Integer Modulo);
bool coprime (Integer a,  Integer b);

char *randstring(); // random number generator, only for test 

#endif /* defined(__Fiat_Shamir__FS_aux_fun__) */
