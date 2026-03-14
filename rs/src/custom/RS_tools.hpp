#ifndef RS_TOOLS_H
#define RS_TOOLS_H
#include <vector>
#include <cstdint>
#include <stdexcept>

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
    int size;           // ordre des éléments : 2^m
    int prim_poly;      // Polynôme primitif P(X)
    std :: vector<int> alpha_to; // hash_map: alpha^i -> polynomial form
    std :: vector<int> index_of; // hash_map : logarithme discret polynomial form -> exponent i
    
public:
    GaloisField(int m);
    
    inline int mul(int a, int b) {
        if (a == 0 || b == 0) return 0;
        int log_a = index_of[a];
        int log_b = index_of[b];
        int log_sum = (log_a + log_b);
        return alpha_to[log_sum % (size -1)];
    }
    inline int div(int a, int b) {
        if (b == 0) throw std :: invalid_argument("Division by zero");
        if (a == 0) return 0;
        int log_a = index_of[a];
        int log_b = index_of[b];
        int log_diff = (log_a - log_b + (size - 1)) % (size - 1);
        return alpha_to[log_diff];
    }

    inline int add(int a, int b) { return a ^ b; }
    inline int sub(int a, int b) { return a ^ b; }

    inline int pow_gf(int a, int n) {
        if (a == 0) return 0;
        int log_a = index_of[a];
        int log_res = (log_a * (n % (size - 1))) % (size - 1);
        return alpha_to[log_res];
    }

    const std :: vector<int>& get_alpha_to() const;
    const std :: vector<int>& get_index_of() const;
    int get_size() const;
    int get_m() const;
    int get_prim_poly() const;
};

#endif