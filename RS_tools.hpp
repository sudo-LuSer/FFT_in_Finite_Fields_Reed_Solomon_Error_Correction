#ifndef RS_TOOLS_H
#define RS_TOOLS_H

#include <bits/stdc++.h>
using namespace std; 

const uint32_t poly_masks[21] = {
    0,          // m=0 (non utilisé)
    0,          // m=1 (non utilisé)
    0x7,        // m=2: 1 + X + X² (111)
    0xB,        // m=3: 1 + X + X³ (1011)
    0x13,       // m=4: 1 + X + X⁴ (10011)
    0x25,       // m=5: 1 + X² + X⁵ (100101)
    0x43,       // m=6: 1 + X + X⁶ (1000011)
    0x83,       // m=7: 1 + X + X⁷ (10000011)
    0x11D,      // m=8: 1 + X³ + X⁴ + X⁵ + X⁸ (100011101) 
    0x211,      // m=9: 1 + X⁴ + X⁹ (1000010001)
    0x409,      // m=10: 1 + X³ + X¹⁰ (10000001001)
    0x805,      // m=11: 1 + X² + X¹¹ (100000000101)
    0x1053,     // m=12: 1 + X + X⁴ + X⁶ + X¹² (1000001010011) 
    0x201B,     // m=13: 1 + X + X³ + X⁴ + X¹³ (10000000011011)
    0x4443,     // m=14: 1 + X + X⁶ + X¹⁰ + X¹⁴ (100010001000011) 
    0x8003,     // m=15: 1 + X + X¹⁵ (1000000000000011)
    0x1100B,    // m=16: 1 + X + X³ + X¹² + X¹⁶ (10001000000001011) 
    0x80009,    // m=17: 1 + X³ + X¹⁷ (100000000000000001001)
    0x100065,   // m=18: 1 + X + X⁵ + X⁶ + X¹⁸ (100000000000001100101) 
    0x200009,   // m=19: 1 + X³ + X¹⁹ (10000000000000000001001)
    0x800021    // m=20: 1 + X + X²⁰ (1000000000000000000100001) 
};

class GaloisField {
    private:
        int m;              // Degree of the field 
        int size;           // ordre des elements : 2^m
        int prim_poly;      // Polynome primitive P(X)
        vector<int> alpha_to; // hash_map: alpha^i -> polynomial form
        vector<int> index_of; // hash_map : logarithme discret polynomial form -> exponent i
        
    public:
        GaloisField(int m) : m(m), prim_poly(poly_masks[m]) {
            size = 1 << m;
            alpha_to.resize(size);
            index_of.resize(size);

            // 1. Initialize tables
            alpha_to[0] = 1; // alpha^0 = 1
            index_of[0] = -INT_MAX; // log(0) is undefined

            // construction de Corps de Galois 
            int val = 1;
            for (int i = 0; i < size - 1; i++) {
                alpha_to[i] = val;
                index_of[val] = i;
                val <<= 1;
                // If we exceed the degree of the primitive polynomial we go forward by calculating modulo P(alpha) 
                if (val & size) {
                    val ^= prim_poly;
                }
            }
        }
        
        int mul(int a, int b);
        int div(int a, int b);
        int add(int a, int b);
        int sub(int a, int b);
        int pow_gf(int a,int b); 

        const vector<int>& get_alpha_to() const;
        const vector<int>& get_index_of() const;
        int get_size() const;
        int get_m() const; 
        int get_prim_poly() const;
    };

#endif