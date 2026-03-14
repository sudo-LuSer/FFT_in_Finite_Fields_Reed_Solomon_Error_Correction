#include "RS_Decoder.hpp"
#include <algorithm>

RS_Decoder::RS_Decoder(int n, int k, GaloisField &gf) : n(n), k(k), gf(gf) {
    t = (n - k) / 2;
    AlphaPow_reg.resize(2*t);
    AlphaInv_reg.resize(n);
    for(int i = 0; i < 2*t ; ++i)
        AlphaPow_reg[i] = gf.get_alpha_to()[i+1];
    for (int i = 0; i < n; ++i)
        AlphaInv_reg[i] = gf.div(1, gf.get_alpha_to()[i]);
}

int RS_Decoder::poly_eval(const std::vector<int>& poly, int x) {
    int result = 0;
    for (int i = (int)poly.size() - 1; i >= 0; --i) {
        result = gf.add(gf.mul(result, x),poly[i]); // LFSR
    }
    return result;
}

std::vector<int> RS_Decoder::compute_syndromes(const std::vector<int>& received) {
    std::vector<int> syndromes(2*t, 0);
    for (int i = 0; i < 2*t; i++) {
        syndromes[i] = poly_eval(received, AlphaPow_reg[i]);
    }
    return syndromes;
}

std::vector<int> RS_Decoder::berlekamp_massey(const std::vector<int>& s) {
    int L = 0;
    std::vector<int> C(2*t+1, 0), B(2*t+1, 0);
    C[0] = 1; B[0] = 1;
    int m = 1;
    for (int r = 0; r < 2*t; ++r) {
        int d = s[r];
        for (int i = 1; i <= L; ++i)
            d ^= gf.mul(C[i], s[r-i]); // xor = + Z/2Z
        if (d == 0) {
            ++m;
        } 
        else if (2*L <= r) {
            auto D = C; 
            for (int i = 0; i < (int)B.size() && i+m < (int)C.size(); ++i)
                C[i+m] ^= gf.mul(d, B[i]);
            L = r + 1 - L;
            B = D;
            m = 1;
            int inv_d = gf.div(1, d);
            for (int &val : B) val = gf.mul(val, inv_d);
        } 
        else{
            for (int i = 0; i < (int)B.size() && i+m < (int)C.size(); ++i)
                C[i+m] ^= gf.mul(d, B[i]);
            ++m;
        }
    }
    C.resize(L+1);
    return C;
}

std::vector<int> RS_Decoder::chien_search(const std::vector<int>& lambda, std::vector<int>& error_positions){
    error_positions.clear();
    std::vector<int> X;
    for (int i = 0; i < n; i++) {
        if (poly_eval(lambda, AlphaInv_reg[i]) == 0) {
            error_positions.push_back(i);
            X.push_back(AlphaInv_reg[i]);             
        }
    }
    return X;
}

std::vector<int> formal_derivative(const std::vector<int>& poly) {
    std::vector<int> deriv(poly.size() - 1, 0);
    for (size_t i = 1; i < poly.size(); i+=2) {
        deriv[i-1] = poly[i]; 
    }
    return deriv;
}

std::vector<int> RS_Decoder::forney(const std::vector<int>& lambda, const std::vector<int>& omega, const std::vector<int>& error_positions, const std::vector<int>& X) {
    std::vector<int> error_values(error_positions.size());
    std::vector<int> lambda_prime = formal_derivative(lambda);
    int idx = 0 ;
    for (auto Xi : X) {
        int omega_eval = poly_eval(omega, Xi);
        int df_lambda_eval = poly_eval(lambda_prime, Xi);
        int numer = omega_eval;
        int denom = df_lambda_eval;
        error_values[idx] = denom == 0 ? 0 : gf.div(numer, denom);
        idx++;
    }
    return error_values;
}

std::vector<int> RS_Decoder::decode(const std::vector<int>& received) {
    std::vector<int> syndromes = compute_syndromes(received);
    bool all_zero = true;
    for (int s : syndromes){
        if (s != 0){
            all_zero = false; 
            break;
        }
    }
    if (all_zero) 
        return received;
    std::vector<int> lambda = berlekamp_massey(syndromes);

    std::vector<int> omega = poly_mult(syndromes, lambda);
    omega.resize(2*t); // mod x^{2t}

    std::vector<int> error_positions;
    std::vector<int> X = chien_search(lambda, error_positions);

    // if (error_positions.size() != lambda.size() - 1) {
    //     throw std::runtime_error("Decoding failure: number of errors mismatch");
    // }

    std::vector<int> error_values = forney(lambda, omega, error_positions, X);

    std::vector<int> corrected = received;
    for (size_t idx = 0; idx < error_positions.size(); ++idx) {
        int pos = error_positions[idx];
        corrected[pos] = gf.sub(corrected[pos], error_values[idx]);
    }

    return corrected;
}

std::vector<int> RS_Decoder::poly_mult(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> res(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == 0) continue;
        for (size_t j = 0; j < b.size(); ++j) {
            if (b[j] == 0) continue;
            res[i+j] = gf.add(res[i+j], gf.mul(a[i], b[j]));
        }
    }
    return res;
} // peut etre à améliorer avec un FFT 