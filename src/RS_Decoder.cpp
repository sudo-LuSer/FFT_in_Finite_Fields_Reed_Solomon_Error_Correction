#include "RS_Decoder.hpp"
#include <algorithm>

RS_Decoder::RS_Decoder(int n, int k, const GaloisField& gf)
    : n(n), k(k), t((n - k) / 2), gf(gf)
{
    syndrome.resize(2 * t);
    lambda.resize(2 * t + 1);
    B.resize(2 * t + 1);
}

void RS_Decoder::compute_syndrome(const std::vector<int>& r)
{
    std::fill(syndrome.begin(), syndrome.end(), 0);

    for (int i = 0; i < n; ++i) {
        int ri = r[i];
        if (ri == 0) continue;

        for (int j = 0; j < 2 * t; ++j) {
            int exp = (i * (j + 1)) % (gf.get_size() - 1);
            syndrome[j] ^= gf.mul(ri, gf.get_alpha_to()[exp]);
        }
    }
}

void RS_Decoder::berlekamp_massey()
{
    std::fill(lambda.begin(), lambda.end(), 0);
    std::fill(B.begin(), B.end(), 0);

    lambda[0] = B[0] = 1;

    int L = 0, m = 1;

    for (int r = 0; r < 2 * t; ++r) {
        int d = syndrome[r];

        for (int i = 1; i <= L; ++i)
            d ^= gf.mul(lambda[i], syndrome[r - i]);

        if (d != 0) {
            std::vector<int> T = lambda;

            for (int i = 0; i <= 2 * t - m; ++i)
                lambda[i + m] ^= gf.mul(d, B[i]);

            if (2 * L <= r) {
                L = r + 1 - L;
                int inv_d = gf.inv(d);

                for (int i = 0; i <= 2 * t; ++i)
                    B[i] = gf.mul(T[i], inv_d);

                m = 1;
            } else {
                m++;
            }
        } else {
            m++;
        }
    }
}

std::vector<int> RS_Decoder::decode(const std::vector<int>& r)
{
    compute_syndrome(r);

    bool error = false;
    for (auto s : syndrome)
        if (s != 0) error = true;

    if (!error) return r;

    berlekamp_massey();

    // (simplified: no Forney here)
    return r;
}