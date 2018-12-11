//
//  FiatShamirProtocol.h
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
//version V.5.0.1

#pragma once
#include <optional>
#include "BigInteger.hpp"

namespace FiatShamirProtocol
{
    using namespace BigInt;

    class Generator
    {
    public:
        virtual BigInteger getBig(unsigned int size)=0;//return a positive BigInteger of size bit
        virtual bool getBit()=0;
    };

    class PrivateKey;
    class PublicKey;

    class Proover
    {
    private:
        const BigInteger key;
        const BigInteger modulus;
        BigInteger sessionNumber;
        Generator &generator;
        const unsigned size;
        bool synch;

        //generator = random number generator, use an your own implementation of the abstract class Generator or my TestGenerator only for test
        Proover(const BigInteger &privkey, const BigInteger &modulus, Generator &generator);

    public:
        //Start the protocol and return the session number to send to Verifier
        BigInteger step1();
        
        //Take result of verifier step1, a random bool, ad return the proof to send to Verifier
        //return 0 if an error is occurred
        BigInteger step2(bool choice);

        friend PrivateKey;
    };
    
    class Verifier
    {
    private:
        bool choice;
        const BigInteger key;
        const BigInteger modulus;
        BigInteger sessionNumber;
        Generator &generator;
        bool state;
        bool synch;

        Verifier(const BigInteger &pubkey, const BigInteger &modulus, Generator &generator);

    public:
        //take result of Proover step1 and return a random choice to send to Proover
        bool step1(BigInteger &sessionNumber);
        
        //take retult of Proover step2, change the state of identification and return this
        bool step2(BigInteger proof);
        
        //return state of identification
        bool checkstate();

        friend PublicKey;
    };

    class PublicKey
    {
        
        

        PublicKey(const BigInteger &key, const BigInteger &modulus);

    public:
        Buffer toBytes() const;
        static std::optional<PublicKey> fromBytes(const Buffer &data);
        Verifier getVerifier(Generator &generator) const;
        std::optional<PrivateKey> restorePrivateKey(const BigInteger &secretNumber);

        bool operator==(const PublicKey &rhs) const;
        bool operator!=(const PublicKey &rhs) const;
const BigInteger key;
const BigInteger modulus;
        friend PrivateKey;
    };

    class PrivateKey
    {
                const BigInteger modulus;

        PrivateKey(const BigInteger &key, const BigInteger &modulus);

    public:
        //size = number of bit of the keys
        static PrivateKey keyGen(Generator &generator, unsigned size);
        static std::optional<PrivateKey> keyGen(const BigInteger &secretNumber, Generator &generator, unsigned size);
        Proover getProover(Generator &generator)const;
        PublicKey getPublicKey();
        Buffer toBytes()const;
        static std::optional<PrivateKey> fromBytes(const Buffer &data);

        bool operator==(const PrivateKey &rhs) const;
        bool operator!=(const PrivateKey &rhs) const;
        const BigInteger key;

        friend PublicKey;
    };
}
