#include "RS_Encoder.hpp"
#include <cstring>
// #include "mipp.h"

RS_Encoder::RS_Encoder(int n, int k, const GaloisField &gf)
    : n(n), k(k), GaloisField(gf)
{
    if (n <= k) throw std::invalid_argument("n must be greater than k");

    r = n - k;
    t = r / 2;

    parity.resize(r);
}

void RS_Encoder::encode(const std::vector<int>& message, std::vector<int>& codeword) {
    std::fill(parity.begin(), parity.end(), 0); 

    int* __restrict par = parity.data();
    const int* __restrict mt = get_mul_table().data();
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