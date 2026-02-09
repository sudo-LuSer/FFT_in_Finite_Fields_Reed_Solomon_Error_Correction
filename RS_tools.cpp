#include "RS_tools.hpp"
#include <stdexcept>

GaloisField::GaloisField(int m) : m(m), prim_poly(poly_masks[m]) {
    if (m < 2 || m > 20) {
        throw invalid_argument("m must be between 2 and 20");
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

int GaloisField::mul(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int log_a = index_of[a];
    int log_b = index_of[b];
    int log_sum = (log_a + log_b) % (size - 1);
    return alpha_to[log_sum];
}

int GaloisField::div(int a, int b) {
    if (b == 0) throw invalid_argument("Division by zero");
    if (a == 0) return 0;
    int log_a = index_of[a];
    int log_b = index_of[b];
    int log_diff = (log_a - log_b + (size - 1)) % (size - 1);
    return alpha_to[log_diff];
}

int GaloisField::add(int a, int b) { return a ^ b; }
int GaloisField::sub(int a, int b) { return a ^ b; }

int GaloisField::pow_gf(int a, int n) {
    if (a == 0) return 0;
    int log_a = index_of[a];
    int log_res = (log_a * (n % (size - 1))) % (size - 1);
    return alpha_to[log_res];
}

int GaloisField::get_size() const { return size; }
int GaloisField::get_m() const { return m; }
int GaloisField::get_prim_poly() const { return prim_poly; }

const vector<int>& GaloisField::get_alpha_to() const { return alpha_to; }
const vector<int>& GaloisField::get_index_of() const { return index_of; }