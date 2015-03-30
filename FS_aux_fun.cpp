//
//  FS_aux_fun.cpp
//  Fiat-Shamir
//
//  Created by ivan sarno on 24/01/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
// version V.2.0

#include "FS_aux_fun.h"

Integer fastexp(Integer base, Integer esp)
{
	if (esp == 0)
		return 1;
	if (esp == 1)
		return base;
	else
	{
		Integer i = 1;
		int j = 0;
		Integer *tempris = new Integer[Block_size];
		tempris[0] = base;
		while (i < esp)
		{
			tempris[j + 1] = tempris[j] * tempris[j];
			j++;
			i *= 2;
		}
		Integer result = 1;
		while (esp > 0)
		{
			if (esp - i >= 0)
			{
				result *= tempris[j];
				esp -= i;
			}
			j--;
			i /= 2;
		}
		delete[] tempris;
		return result;
	}
}

Integer modexp(Integer base, Integer esp, Integer mod)
{
	if (esp == 0)
		return 1;
	if (esp == 1)
		return base;
	else
	{
		Integer i = 1;
		int j = 0;
		Integer *tempris = new Integer[Block_size];
		tempris[0] = base;
		while (i < esp)
		{
			tempris[j + 1] = (tempris[j] * tempris[j]) % mod;
			j++;
			i *= 2;
		}
		Integer result = 1;
		while (esp > 0)
		{
			if (esp - i >= 0)
			{
				result = (result * tempris[j]) % mod;
				esp -= i;
			}
			j--;
			i /= 2;
		}
		delete[] tempris;
		return result;
	}
}

bool MRpredicate1 (Integer y, Integer z, Integer N)
{
    if (modexp(y,z,N)==1)
        return true;
    else return false;
}

bool MRpredicate2(Integer y, Integer N, Integer z, Integer w)
{
    Integer i=0;
    bool cond = (modexp(y,fastexp(2,i)*z, N) == N-1);
    
    while (!cond && i < w)
    {
        i++;
        cond = (modexp(y,fastexp(2,i)*z, N) == N-1);
    }
    
    if (i==w)
        return false;
    else return true;
    
}

bool MRtest(Integer N)  //Miller-rabin test for prime number
{
    Integer w; Integer z;
    
    MRscomposition(N,w,z);
    
    bool ris =true;
    int i=0;
    
    
    Integer y;
    
    while (ris && i < Precision)
    {
        y = RandNum;
        y= y % N;
        while(y<2)
        {
            y = RandNum;
            y= y % N;
        }
        ris = (coprime(y,N)) && (MRpredicate1(y, z, N)|| MRpredicate2(y, N, z, w));
        i++;
    }
    return ris;
}

void MRscomposition(Integer N, Integer &w, Integer &z)
{
    Integer i=1;
    Integer acc=2;
    N--;
    Integer r;    
    while (acc < N)
    {
        r=N/acc;
        if ((N%acc==0) && (r%2==1))
        {
            w=i;
            z=r;
        }
        i++;
        acc = acc * 2;
    }
}

//extract a random number and search a early prime
Integer Primegenerator()
{
    Integer P = RandNum;
    if (P%2==0)
        P++;
    
    while (!MRtest(P))//test primality
    {
        P = P+2;
    }
    
    return P;
}

/*
 implementation of Extended Euclide algorithm to find the inverse of a numbere in module and coprime number
 */

typedef struct
{
    Integer x;
    Integer y;
    Integer z;
} triple;




triple ExtendedEuclide(Integer a, Integer b)
{
    triple result;
    triple temp;
    
    if (b == 0)
    {
        result.x = a;
        result.y = 1;
        result.z = 0;
        
        return result;
    }
    else temp = ExtendedEuclide(b, a % b);
    
    result.x = temp.x;
    result.y = temp.z;
    result.z = temp.y - ((a / b) * temp.z);
    
    return result;
}

Integer inverso(Integer n,Integer Modulo)
{
    triple ris = ExtendedEuclide(n,Modulo);
    if (ris.y<0)
        ris.y=Modulo+ris.y;
    
    return  ris.y;
}

bool coprime (Integer a, Integer b)
{
    triple temp = ExtendedEuclide(a, b);
    if (temp.x == 1)
        return true;
    else return false;
}

//generates a string of random digit, isn't sure, for test only
char *randstring()
{
	int size = (Block_size * 3) / 20 - 1;
	char * rs = new char[size + 1];
	int i;
	for (i = 0; i < size; i++)
		rs[i] = (char)(rand() % 10 + 48);

	rs[size] = '\0';
	return rs;
}