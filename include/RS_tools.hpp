#pragma once

#ifndef RS_TOOLS_H
#define RS_TOOLS_H
#include <vector>
#include <cstdint>
#include <stdexcept>

#define GF_ADD(a,b) ((a) ^ (b))
#define GF_SUB(a,b) ((a) ^ (b))

extern const uint32_t poly_masks[21];

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
        int log_sum = (log_a + log_b) % (size - 1);
        return alpha_to[log_sum];
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
    inline int inv(int a) { return alpha_to[(size - 1) - index_of[a]]; }
    inline int pow_gf(int a, int n) {
        if (a == 0) return 0;
        int log_a = index_of[a];
        int log_res = (log_a * (n % (size - 1))) % (size - 1);
        return alpha_to[log_res];
    }
    const std::vector<int>& get_alpha_to() const;
    const std::vector<int>& get_index_of() const;
    int get_size() const;
    int get_m() const;
    int get_prim_poly() const;
};

#endif