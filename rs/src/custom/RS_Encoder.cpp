#include "RS_Encoder.hpp"
#include <cstring>

// #include "mipp.h"

RS_Encoder::RS_Encoder(int n, int k, const GaloisField &gf)
    : n(n), k(k), GaloisField(gf)
{
    if (n <= k) throw std::invalid_argument("n must be greater than k");

    r = n - k;
    int sz = size;

    t = r / 2;

    generator.resize(r + 1, 0);
    generator[0] = 1;

    for (int i = 1; i <= r; ++i) {
        int alpha_i = alpha_to[i];

        for (int j = i; j > 0; --j) {
            generator[j] = GF_ADD(generator[j - 1], mul(generator[j], alpha_i));
        }

        generator[0] = mul(generator[0], alpha_i);
    }

    mul_table.resize(size * r);

    int* mt = mul_table.data();

    for (int x = 0; x < size; ++x) {
        for (int j = 0; j < r; ++j) {
            mt[x * r + j] = mul(x, generator[j]);
        }
    }

    parity.resize(r, 0);
}

void RS_Encoder::encode(const std::vector<int>& message, std::vector<int>& codeword) {
    std::fill(parity.begin(), parity.end(), 0); 

    int* __restrict par = parity.data();
    const int* __restrict mt = mul_table.data();
    const int* __restrict msg = message.data();
    
    for (int i = k - 1; i >= 0; --i) {
        int feedback = GF_ADD(msg[i], par[r-1]);
        for (int j = r - 1; j > 0; --j) {
            par[j] = GF_ADD(par[j-1], mt[feedback * r + j]);
        }
        par[0] = mt[feedback * r];
    }
    
    codeword.resize(n);
    std::memcpy(codeword.data(), par, r * sizeof(int));
    std::memcpy(codeword.data() + r, msg, k * sizeof(int));
}

// void RS_Encoder::encode(const std::vector<int>& message,
//                         std::vector<int>& codeword)
// {
//     constexpr int N = mipp::N<int>();
//     int* __restrict par = parity.data();
//     const int* __restrict mt = mul_table.data();
//     const int* __restrict msg = message.data();

//     for (int i = 0; i < k; ++i) {
//         int feedback = msg[i] ^ par[0];

//         for (int j = 0; j < r - 1; ++j)
//             par[j] = par[j + 1];
//         par[r - 1] = 0;

//         if (feedback) {
//             const int* row = mt + feedback * r;
//             int j = 0;
//             for (int vec_end = (r / N) * N; j < vec_end; j += N) {
//                 mipp::Reg<int> p(&par[j]);
//                 mipp::Reg<int> rw(&row[j]);
//                 p ^= rw;
//                 p.store(&par[j]);
//             }
//             for (; j < r; ++j)
//                 par[j] ^= row[j];
//         }
//     }

//     codeword.resize(n);
//     // std::copy(par, par + r, codeword.begin());
//     // std::copy(message.begin(), message.end(), codeword.begin() + r);
//     std::memcpy(codeword.data(), par, r * sizeof(int));
//     std::memcpy(codeword.data() + r, msg, k * sizeof(int));
// }