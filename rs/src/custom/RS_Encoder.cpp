#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include <assert.h>
RS_Encoder::RS_Encoder(int n, int k, GaloisField &gf) : n(n), k(k), gf(&gf) {
    if (n <= k) throw std::invalid_argument("n must be greater than k");
    t = (n - k) / 2;

    int r = n - k;
    generator.resize(r + 1);
    generator[0] = gf.get_alpha_to()[r*(r+1)/2 % (gf.get_size() - 1)];
    for (int i = 1; i <= r; ++i) {
        int alpha_i = gf.get_alpha_to()[i];
        for (int j = generator.size() - 1; j > 0; --j) {
            generator[j] = GF_ADD(generator[j-1], gf.mul(generator[j], alpha_i));
        }
    }

    int sz = gf.get_size();
    mul_table.resize(r * sz);
    int *mt = mul_table.data();
    for (int j = 0; j < r; ++j) {
        int gen_j = generator[j];
        for (int x = 0; x < sz; ++x) {
            mt[j * sz + x] = gf.mul(x, gen_j);
        }
    }

}

void RS_Encoder::encode(const std::vector<int>& message, std::vector<int>& codeword) {
    const int r = n - k;
    const int sz = gf->get_size();
    const int *mt = mul_table.data();
    std::vector<int> parity(r, 0);
    int *par = parity.data();
    
    for (int i = k - 1; i >= 0; --i) {
        int feedback = GF_ADD(message[i], par[r-1]);
        for (int j = r - 1; j > 0; --j) {
            par[j] = GF_ADD(par[j-1], mt[j * sz + feedback]);
        }
        par[0] = mt[feedback]; 
    }
    
    codeword.resize(n);
    std::copy(par, par + r, codeword.begin());
    std::copy(message.begin(), message.end(), codeword.begin() + r);
}