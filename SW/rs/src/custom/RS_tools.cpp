#include "RS_tools.hpp"

const uint32_t poly_masks[21] = {
    0,0,0x7,0xB,0x13,0x25,0x43,0x83,0x11D,
    0x211,0x409,0x805,0x1053,0x201B,0x4443,
    0x8003,0x1100B,0x80009,0x100065,0x200009,0x800021
};

GaloisField::GaloisField(int m) : m(m), prim_poly(poly_masks[m]) {
    if (m < 2 || m > 20)
        throw std::invalid_argument("m must be between 2 and 20");

    size = 1 << m;

    alpha_to.resize(size);
    index_of.resize(size);

    int val = 1;
    for (int i = 0; i < size - 1; ++i) {
        alpha_to[i] = val;
        index_of[val] = i;
        val <<= 1;
        if (val & size) val ^= prim_poly;
    }

    index_of[0] = -1;

}

void GaloisField :: dp_mt(const int r, const std::vector<int>& generator) {
    mul_table.resize(size * r);

    int* mt = mul_table.data();

    for (int x = 0; x < size; ++x) {
        for (int j = 0; j < r; ++j) {
            mt[x * r + j] = mul(x, generator[j]);
        }
    }
}


void GaloisField :: define_generator(const int r) {
    generator.resize(r + 1);
    generator[0] = 1;

    for (int i = 1; i <= r; ++i) {
        int alpha_i = alpha_to[i];

        for (int j = i; j > 0; --j) {
            generator[j] = GF_ADD(generator[j - 1], mul(generator[j], alpha_i));
        }

        generator[0] = mul(generator[0], alpha_i);
    }

    dp_mt(r, generator);
}

int GaloisField::get_m() const { return m; }
int GaloisField::get_prim_poly() const { return prim_poly; }
int GaloisField::get_size() const { return size; }


const std::vector<int>& GaloisField::get_alpha_to() const { return alpha_to; }
const std::vector<int>& GaloisField::get_index_of() const { return index_of; }
const std::vector<int>& GaloisField::get_generator() const { return generator; }
const std::vector<int>& GaloisField::get_mul_table() const { return mul_table; }