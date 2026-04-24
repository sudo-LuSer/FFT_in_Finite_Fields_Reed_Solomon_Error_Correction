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
    int m;
    int prim_poly;
protected:
    int size;

    std :: vector<int> alpha_to;
    std :: vector<int> index_of;
    std :: vector<int> generator;
    std :: vector<int> mul_table;
public:
    GaloisField(int m);

    int get_m() const;
    int get_prim_poly() const;
    int get_size() const;

    const std::vector<int>& get_alpha_to() const;
    const std::vector<int>& get_index_of() const;
    const std::vector<int>& get_mul_table() const;
    const std::vector<int>& get_generator() const;
    void define_generator(const int r);
    void dp_mt(const int r, const std::vector<int>& generator);

    // inline ops 
    inline int mul(int a, int b) const {
        if (a == 0 || b == 0) return 0;
        return alpha_to[(index_of[a] + index_of[b]) % (size - 1)];
    }

    inline int div(int a, int b) const {
        if (b == 0) throw std::invalid_argument("Division by zero");
        if (a == 0) return 0;
        return alpha_to[(index_of[a] - index_of[b] + (size - 1)) % (size - 1)];
    }

    inline int inv(int a) const {
        return alpha_to[(size - 1) - index_of[a]];
    }
};

#endif