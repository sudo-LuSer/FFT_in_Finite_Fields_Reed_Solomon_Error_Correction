#include "RS_tools.hpp"

GaloisField::GaloisField(int m) : m(m), prim_poly(poly_masks[m]) {
    if (m < 2 || m > 20) {
        throw std :: invalid_argument("m must be between 2 and 20");
    }
    
    size = 1 << m;
    alpha_to.resize(size);
    index_of.resize(size);

    // 1. Initialize tables
    alpha_to[0] = 1; // alpha^0 = 1
    index_of[0] = -1; // log(0) is undefined, use -1 instead of -INT_MAX

    // construction du Corps de Galois 
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

int GaloisField::get_size() const { return size; }
int GaloisField::get_m() const { return m; }
int GaloisField::get_prim_poly() const { return prim_poly; }

const std :: vector<int>& GaloisField::get_alpha_to() const { return alpha_to; }
const std :: vector<int>& GaloisField::get_index_of() const { return index_of; }