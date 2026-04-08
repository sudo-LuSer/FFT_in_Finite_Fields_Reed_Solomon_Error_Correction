#include "RS_Decoder.hpp"
#include <algorithm>
#include <cstring>

RS_Decoder::RS_Decoder(int n, int k, GaloisField &gf)
    : n(n), k(k), gf(gf) {
    t = (n - k) / 2;
    int m = gf.get_m();
    
    alpha_pow_syn.resize(2*t);
    for (int i = 0; i < 2*t; ++i)
        alpha_pow_syn[i] = gf.get_alpha_to()[i+1];

    alpha_inv.resize(n);
    const auto& alpha_to = gf.get_alpha_to();
    for (int i = 0; i < n; ++i)
        alpha_inv[i] = gf.inv(alpha_to[i]);
    
    // Precompute syndrome evaluation table: for each position pos (0..n-1) and each syndrome index j (0..2t-1)
    // value = alpha_to[ (pos+1)*(j+1) mod (2^m-1) ]? Actually S_j = r(alpha^(j+1)) = sum r_i * alpha^(i*(j+1))
    // We'll precompute alpha_pow_table[pos][j] = alpha^(i * (j+1))
    syndrome_table.resize(n * (2*t));
    int *st = syndrome_table.data();
    int order = gf.get_size() - 1;  // 2^m - 1
    for (int pos = 0; pos < n; ++pos) {
        for (int j = 0; j < 2*t; ++j) {
            int exp = (pos * (j+1)) % order;
            st[pos * (2*t) + j] = alpha_to[exp];
        }
    }
    
    error_flag = false;

    lambda_scratch.resize(2*t+2);
    omega_scratch.resize(2*t+2);
    error_positions_scratch.resize(t);
    X_scratch.resize(t);
}

std::vector<int> RS_Decoder::compute_syndromes(const std::vector<int>& received) {
    std::vector<int> syndromes(2*t, 0);
    const int *rec = received.data();
    const int *st = syndrome_table.data();
    int *syn = syndromes.data();
    
    for (int pos = 0; pos < n; ++pos) {
        int rpos = rec[pos];
        if (rpos == 0) continue;
        const int *row = st + pos * (2*t);
        for (int j = 0; j < 2*t; ++j) {
            syn[j] ^= gf.mul(rpos, row[j]);
        }
    }
    error_flag = std::any_of(syn, syn + 2*t, [](int s){ return s != 0; });
    return syndromes;
}

void RS_Decoder::berlekamp_massey(const std::vector<int>& s, std::vector<int>& lambda_out) {
    const int N = 2*t;
    // Use scratch vectors
    std::vector<int>& C = lambda_scratch;
    std::vector<int>& B = omega_scratch; 
    C.assign(N+1, 0);
    B.assign(N+1, 0);
    C[0] = B[0] = 1;
    int L = 0, m = 1;
    
    for (int r = 0; r < N; ++r) {
        int d = s[r];
        for (int i = 1; i <= L; ++i) {
            if (C[i] && s[r-i])
                d ^= gf.mul(C[i], s[r-i]);
        }
        if (d != 0) {
            // D = C (we'll copy later)
            std::vector<int> D = C;  // unavoidable copy but size <= N+1
            for (int i = 0; i <= N - m; ++i) {
                if (B[i])
                    C[i+m] ^= gf.mul(d, B[i]);
            }
            if (2*L <= r) {
                L = r + 1 - L;
                int inv_d = gf.inv(d);
                for (int i = 0; i <= N; ++i)
                    B[i] = gf.mul(D[i], inv_d);
                m = 1;
            } else {
                ++m;
            }
        } else {
            ++m;
        }
    }
    lambda_out.assign(C.begin(), C.begin() + L + 1);
}

std::vector<int> RS_Decoder::chien_search(const std::vector<int>& lambda,
                                          std::vector<int>& error_positions) {
    error_positions.clear();
    int L = (int)lambda.size() - 1;
    if (L <= 0) return {};
    
    std::vector<int> reg = lambda; 
    std::vector<int> X;
    X.reserve(t);
    
    for (int i = 0; i < n; ++i) {
        int sum = 0;
        for (int j = 0; j <= L; ++j)
            sum ^= reg[j];
        if (sum == 0) {
            error_positions.push_back(i);
            X.push_back(alpha_inv[i]);  // X = alpha^(-i)
        }
        for (int j = 1; j <= L; ++j)
            reg[j] = gf.mul(alpha_inv[j], reg[j]);
    }
    return X;
}

std::vector<int> RS_Decoder::formal_derivative(const std::vector<int>& poly) {
    std::vector<int> d(poly.size() - 1, 0);
    for (size_t i = 1; i < poly.size(); i += 2)
        d[i-1] = poly[i];
    return d;
}

std::vector<int> RS_Decoder::forney(const std::vector<int>& lambda,
                                    const std::vector<int>& omega,
                                    const std::vector<int>& error_positions,
                                    const std::vector<int>& X) {
    std::vector<int> error_values(X.size());
    std::vector<int> lambda_prime = formal_derivative(lambda);
    for (size_t i = 0; i < X.size(); ++i) {
        int Xi = X[i];
        int num = poly_eval(omega, Xi);
        int den = poly_eval(lambda_prime, Xi);
        error_values[i] = (den == 0) ? 0 : gf.mul(num, gf.inv(den));
    }
    return error_values;
}

// Poly multiplication with early exit (small polynomials)
std::vector<int> RS_Decoder::poly_mult(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> res(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); ++i) {
        int ai = a[i];
        if (ai == 0) continue;
        for (size_t j = 0; j < b.size(); ++j) {
            int bj = b[j];
            if (bj == 0) continue;
            res[i+j] ^= gf.mul(ai, bj);
        }
    }
    return res;
}

// Poly eval with Horner, uses gf.mul (could be inlined)
int RS_Decoder::poly_eval(const std::vector<int>& poly, int x) {
    int result = 0;
    for (int i = (int)poly.size() - 1; i >= 0; --i)
        result = GF_ADD(gf.mul(result, x), poly[i]);
    return result;
}

// Main decode with reuse of scratch
std::vector<int> RS_Decoder::decode(const std::vector<int>& received) {
    std::vector<int> syndromes = compute_syndromes(received);
    if (!error_flag) return received;

    std::vector<int> lambda;
    berlekamp_massey(syndromes, lambda);
    
    std::vector<int> omega = poly_mult(lambda, syndromes);
    omega.resize(2*t);
    
    std::vector<int> error_positions;
    std::vector<int> X = chien_search(lambda, error_positions);
    std::vector<int> error_values = forney(lambda, omega, error_positions, X);
    
    std::vector<int> corrected = received;
    for (size_t i = 0; i < error_positions.size(); ++i)
        corrected[error_positions[i]] ^= error_values[i];
    return corrected;
}