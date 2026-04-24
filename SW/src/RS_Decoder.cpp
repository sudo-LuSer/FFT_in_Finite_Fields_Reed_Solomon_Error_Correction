#include "RS_Decoder.hpp"
#include <algorithm>

#define GF_ADD(a,b) ((a) ^ (b))
#define GF_SUB(a,b) ((a) ^ (b))

RS_Decoder::RS_Decoder(int n, int k, GaloisField &gf)
    : n(n), k(k), gf(gf)
{
    t = (n - k) / 2;

    AlphaPow_reg.resize(2*t);
    AlphaInv_reg.resize(n);

    int* a_pow = AlphaPow_reg.data();
    int *a_inv = AlphaInv_reg.data();

    for (int i = 0; i < 2*t; ++i)
        a_pow[i] = gf.get_alpha_to()[i+1];

    for (int i = 0; i < n; ++i)
        a_inv[i] = gf.inv(gf.get_alpha_to()[i]);
}

inline int RS_Decoder::poly_eval(const std::vector<int>& poly, int x)
{
    int result = 0;

    for (int i = (int)poly.size() - 1; i >= 0; --i)
        result = GF_ADD(gf.mul(result, x), poly[i]);

    return result;
}

std::vector<int> RS_Decoder::compute_syndromes(const std::vector<int>& received)
{
    std::vector<int> synd(2*t);

    for (int i = 0; i < 2*t; ++i)
        synd[i] = poly_eval(received, AlphaPow_reg[i]);

    return synd;
}

std::vector<int> RS_Decoder::berlekamp_massey(const std::vector<int>& s)
{
    const int N = 2*t;

    std::vector<int> C(N+1, 0), B(N+1, 0), D(N+1);

    C[0] = 1;
    B[0] = 1;

    int L = 0, m = 1;

    for (int r = 0; r < N; ++r) {
        int d = s[r];

        for (int i = 1; i <= L; ++i)
            if (C[i] && s[r-i])
                d ^= gf.mul(C[i], s[r-i]);

        if (d != 0) {
            D = C; // std::copy(C.begin(), C.end(), D.begin());

            for (int i = 0; i <= N-m; ++i)
                if (B[i])
                    C[i+m] ^= gf.mul(d, B[i]);

            if (2*L <= r) {
                L = r + 1 - L;

                int inv_d = gf.inv(d);

                for (int i = 0; i <= N; ++i)
                    B[i] = gf.mul(D[i], inv_d);

                m = 1;
            } 
            else {
                ++m;
            }
        } 
        else {
            ++m;
        }
    }

    C.resize(L+1);
    return C;
}

std::vector<int> RS_Decoder::chien_search(
    const std::vector<int>& lambda,
    std::vector<int>& error_positions)
{
    error_positions.clear();

    std::vector<int> X;
    X.reserve(t);

    int L = lambda.size() - 1;

    std::vector<int> reg(lambda);

    for (int i = 0; i < n; i++){
        int sum = 0;

        for (int j = 0; j <= L; j++)
            sum ^= reg[j];

        if (sum == 0) {
            error_positions.push_back(i);
            X.push_back(AlphaInv_reg[i]);
        }

        for (int j = 1; j <= L; j++)
                reg[j] = gf.mul(reg[j], AlphaInv_reg[j]);
    }

    return X;
}

static std::vector<int> formal_derivative(const std::vector<int>& poly){
    std::vector<int> d(poly.size() - 1, 0);

    for (size_t i = 1; i < poly.size(); i += 2)
        d[i-1] = poly[i];

    return d;
}

std::vector<int> RS_Decoder::forney(
    const std::vector<int>& lambda,
    const std::vector<int>& omega,
    const std::vector<int>& error_positions,
    const std::vector<int>& X)
{
    std::vector<int> error_values(X.size());

    std::vector<int> lambda_prime = formal_derivative(lambda);

    for (size_t i = 0; i < X.size(); ++i) {
        int Xi = X[i];

        int num = poly_eval(omega, Xi);
        int den = poly_eval(lambda_prime, Xi);

        if (den == 0)
            error_values[i] = 0;
        else
            error_values[i] = gf.mul(num, gf.inv(den));
    }

    return error_values;
}

std::vector<int> RS_Decoder::poly_mult(
    const std::vector<int>& a,
    const std::vector<int>& b)
{
    std::vector<int> res(a.size() + b.size() - 1, 0);

    for (size_t i = 0; i < a.size(); ++i) {
        if (!a[i]) continue;

        for (size_t j = 0; j < b.size(); ++j) {
            if (!b[j]) continue;

            res[i+j] ^= gf.mul(a[i], b[j]);
        }
    }

    return res;
}

std::vector<int> RS_Decoder::decode(const std::vector<int>& received){
    std::vector<int> synd = compute_syndromes(received);

    bool all_zero = true;
    for (int s : synd) all_zero &= (s == 0);
    if (all_zero) return received;

    std::vector<int> lambda = berlekamp_massey(synd);

    std::vector<int> omega = poly_mult(synd, lambda);
    omega.resize(2*t);

    std::vector<int> error_positions;
    std::vector<int> X = chien_search(lambda, error_positions);

    std::vector<int> error_values = forney(lambda, omega, error_positions, X);

    std::vector<int> corrected = received;

    for (size_t i = 0; i < error_positions.size(); ++i)
        corrected[error_positions[i]] ^= error_values[i];

    return corrected;
}