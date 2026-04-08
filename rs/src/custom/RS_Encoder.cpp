#include "RS_Encoder.hpp"
#include "RS_tools.hpp"

RS_Encoder::RS_Encoder(int n, int k, GaloisField &gf) : n(n), k(k), gf(&gf) {
    if (n <= k) throw std::invalid_argument("n must be greater than k");
    t = (n - k) / 2;

    generator.push_back(1);
    for (int i = 1; i <= (n - k); ++i) {
        int alpha_i = gf.get_alpha_to()[i];
        generator = poly_mult_by_binomial(generator, alpha_i);
    }

    int r = n - k;
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

std::vector<int> RS_Encoder::poly_mult_by_binomial(const std::vector<int>& poly, int a) {
    std::vector<int> result(poly.size() + 1, 0);
    result[0] = gf->mul(poly[0], a);
    for (size_t i = 1; i < poly.size(); ++i)
        result[i] = GF_ADD(gf->mul(poly[i], a), poly[i-1]);
    result[poly.size()] = poly.back();
    return result;
}

void RS_Encoder::encode(const std::vector<int>& message, std::vector<int>& codeword) {
    if (message.size() != (size_t)k)
        throw std::invalid_argument("Message length must be k");

    const int r = n - k;
    const int size = gf->get_size();
    const int *mt = mul_table.data();
    std::vector<int> parity(r, 0);
    int *par = parity.data();

    for (int i = k - 1; i >= 0; --i) {
        int feedback = GF_ADD(message[i], parity[r-1]);
        for (int j = r - 1; j > 0; --j) {
            parity[j] = GF_ADD(parity[j-1], mul_table[j * size + feedback]);
        }
        parity[0] = mt[feedback];
    }

    codeword.resize(n);
    std::copy(parity.begin(), parity.end(), codeword.begin());
    std::copy(message.begin(), message.end(), codeword.begin() + r);
}