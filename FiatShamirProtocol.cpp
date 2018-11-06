//
//  FiatShamirProtocol.cpp
//  Zero Knowledge Fiat-Shamir Protocol
//
//  Created by ivan sarno on 24/08/15.
//  Copyright (c) 2015 ivan sarno.
/*
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
//version V.5.0

#include "FiatShamirProtocol.hpp"

using namespace FiatShamirProtocol;

static bool keyCheck(const BigInteger &number, unsigned size)
{
    return number > 2;
}

static bool sessionNumberCheck(const BigInteger &number, unsigned size)
{
    return number > 2;
}

BigInteger Proover::step1()
{
    sessionNumber = mod(generator.getBig(size), modulus);
    synch = true;
    
    while(!sessionNumberCheck(sessionNumber, size))//avoid comunication of the key
        sessionNumber = mod(generator.getBig(size), modulus);
    
    return mod(sessionNumber * sessionNumber, modulus);
}


BigInteger Proover::step2(bool choice)
{
    if(!synch)
        return 0; //0 is an error
    if(choice)
        return mod(sessionNumber * key, modulus);
    return sessionNumber;
}

Proover::Proover(const BigInteger &privkey, const BigInteger &modulus, Generator &generator): key(privkey), modulus(modulus),
    size(bitSize(modulus)), generator(generator) {}

bool Verifier::step1(BigInteger &sessionNumber) //take result of Proover step1
{
    if(sessionNumber < 2) //avoid attack
    {
        this->sessionNumber=1;
        return false;
    }
    this->sessionNumber = sessionNumber;
    synch = true;
    state = false;
    choice=generator.getBit();
    return choice;
}

bool Verifier::step2(BigInteger proof) //take result of Proover step2 and change the state
{
    if(!synch)
    {
        state = false;
        return false;
    }
    synch = false;
    proof = mod(proof*proof, modulus);
    
    BigInteger y;
    
    if (choice)
        y = mod(sessionNumber * key, modulus);
    else y = sessionNumber;
    
    state = proof==y;
    
    return state;
}

bool Verifier::checkstate() //return state of identification
{
    return state;
}

Verifier::Verifier(const BigInteger &pubkey, const BigInteger &modulus, Generator &generator): key(pubkey), modulus(modulus),
generator(generator) {}


PrivateKey::PrivateKey(const BigInteger &key, const BigInteger &modulus) : key(key), modulus(modulus) {}

bool PrivateKey::operator==(const PrivateKey &rhs) const
{
    return key == rhs.key &&
           modulus == rhs.modulus;
}

bool PrivateKey::operator!=(const PrivateKey &rhs) const
{
    return !(rhs == *this);
}

Proover PrivateKey::getProover(Generator &generator) const
{
    return Proover(key, modulus, generator);
}

PublicKey PrivateKey::getPublicKey()
{
    return PublicKey(mod(key*key, modulus), modulus);
}

Buffer PrivateKey::toBytes() const
{
    auto buffer = Buffer();
    buffer.reserve(bitSize(key)/8);
    bigIntegerToBytes(key, buffer);
    bigIntegerToBytes(modulus, buffer);
    return buffer;
}

std::optional<PrivateKey> PrivateKey::fromBytes(const Buffer &data)
{
    size_t index = 0;
    BigInteger key = bigIntegerFromBytes(data, index);
    if(key < 0)
        return std::optional<PrivateKey>();
    BigInteger modulus = bigIntegerFromBytes(data, index);
    if(modulus < 0)
        return std::optional<PrivateKey>();
    return PrivateKey(key, modulus);
}

std::optional<PrivateKey> PrivateKey::keyGen(const BigInteger &secretNumber, Generator &generator, unsigned size)
{
    if(size < 1024 || !keyCheck(secretNumber, size))
        return std::optional<PrivateKey>();

    unsigned primeSize = size/2;
    BigInteger temp = 0;
    do
    {
        temp = generator.getBig(primeSize);
        while(bitSize(temp) < primeSize-2)
            temp = generator.getBig(primeSize);
        temp = nextPrime(temp);
    }while(!isPrime(temp, 40));
    BigInteger primeP = temp;
    temp = 0;
    do
    {
        temp = generator.getBig(primeSize);
        while(bitSize(temp) < primeSize-2)
            temp = generator.getBig(primeSize);
        temp = nextPrime(temp);
    }while(!isPrime(temp, 40) || bitSize(abs(primeP-temp)) < 32);
    BigInteger primeQ = temp;
    
    BigInteger modulus = primeP * primeQ;

    return PrivateKey(secretNumber, modulus);
}

PrivateKey PrivateKey::keyGen(Generator &generator, unsigned size)
{
    if(size < 1024)
        size = 1024;

    unsigned primeSize = size/2;
    BigInteger temp = 0;
    do
    {
        temp = generator.getBig(primeSize);
        while(bitSize(temp) < primeSize-2)
            temp = generator.getBig(primeSize);
        temp = nextPrime(temp);
    }while(!isPrime(temp, 40));
    BigInteger primeP = temp;
    temp = 0;
    do
    {
        temp = generator.getBig(primeSize);
        while(bitSize(temp) < primeSize-2)
            temp = generator.getBig(primeSize);
        temp = nextPrime(temp);
    }while(!isPrime(temp, 40) || bitSize(abs(primeP-temp)) < 32);
    BigInteger primeQ = temp;
    
    BigInteger modulus = primeP * primeQ;
    BigInteger key = mod(generator.getBig(size), modulus);
    while(!keyCheck(key, size))
        key = mod(generator.getBig(size), modulus);
    return PrivateKey(key, modulus);
}

PublicKey::PublicKey(const BigInteger &key, const BigInteger &modulus) : key(key), modulus(modulus) {}

bool PublicKey::operator==(const PublicKey &rhs) const
{
    return key == rhs.key &&
           modulus == rhs.modulus;
}

bool PublicKey::operator!=(const PublicKey &rhs) const
{
    return !(rhs == *this);
}

Buffer PublicKey::toBytes() const
{
    auto buffer = Buffer();
    buffer.reserve(bitSize(key));
    bigIntegerToBytes(key, buffer);
    bigIntegerToBytes(modulus, buffer);
    return buffer;
}

std::optional<PublicKey> PublicKey::fromBytes(const Buffer &data)
{
    size_t index = 0;
    BigInteger key = bigIntegerFromBytes(data, index);
    if(key < 0)
        return std::optional<PublicKey>();
    BigInteger modulus = bigIntegerFromBytes(data, index);
    if(modulus < 0)
        return std::optional<PublicKey>();
    return PublicKey(key, modulus);
}

Verifier PublicKey::getVerifier(Generator &generator) const
{
    return Verifier(key, modulus, generator);
}

std::optional<PrivateKey> PublicKey::restorePrivateKey(const BigInteger &secretNumber)
{
    if(mod(secretNumber*secretNumber, modulus) == key)
        return PrivateKey(secretNumber, modulus);
    return std::optional<PrivateKey>();
}


