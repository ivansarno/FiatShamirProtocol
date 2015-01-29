//
//  Define.h
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//

#ifndef Fiat_Shamir_Define_h
#define Fiat_Shamir_Define_h

#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "ttmath/ttmath.h" //arbitrary precision library




#define Intero ttmath::Int<32> //set int size of block: 32 for 1024 bit, 64 for 2048
#define Primegen Primegenerator() //prime number generator
#define Precision 15 //precision of Miller-Rabin test
#define Distance 65536 // distance between prime number (2^16)
#define Randinit srand((int) time(NULL)); //initializator of random number generator
#define RandNum rand() //random number generator
#define Randbit rand() % 2; //random int [0,1] generator
#define Randbitinit srand((int) time(NULL)); //initilizato of Randbit
#endif
