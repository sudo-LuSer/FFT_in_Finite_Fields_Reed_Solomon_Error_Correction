#include "RS_Decoder.hpp"
#include <algorithm>

RS_Decoder::RS_Decoder(int n, int k, GaloisField &gf) : n(n), k(k), gf(gf) {
    t = (n - k) / 2;
}

std::vector<int> RS_Decoder::compute_syndromes(const std::vector<int>& received) {
    std::vector<int> syndromes(2*t, 0);
    for (int i = 0; i < 2*t; i++) {
        int alpha_i = gf.get_alpha_to()[(i+1) % (gf.get_size() - 1)]; 
        syndromes[i] = poly_eval(received, alpha_i);
    }
    return syndromes;
}

std::vector<int> RS_Decoder::berlekamp_massey(const std::vector<int>& syndromes) {
    int L = 0; 
    std::vector<int> lambda(2*t + 1, 0);
    lambda[0] = 1;
    std::vector<int> b(2*t + 1, 0);
    b[0] = 1;
    int m = 1;
    for (int r = 0; r < 2*t; r++) { // Voir algo dans le doc
        int d = syndromes[r];
        for (int i = 1; i <= L; i++) {
            d = GF_ADD(d, gf.mul(lambda[i], syndromes[r - i]));
        }
        if(d==0){
            m++;
        }
        else if(2*L <= r){
            std::vector<int> T = lambda;
            for (int i = 0; (i<b.size()) && ((i + m) < lambda.size()); i++){
                lambda[i + m] = GF_SUB(lambda[i + m], gf.mul(d, b[i]));
            }
            L = r + 1 - L; b = T; m = 1; 

            int inv_d = gf.div(1, d);
            for (int &val : b)
                val = gf.mul(val, inv_d);
        } 
        else{
            for (int i = 0; (i<b.size()) && ((i + m) < lambda.size()); i++){
                lambda[i + m] = GF_SUB(lambda[i + m], gf.mul(d, b[i]));
            }
            m++;
        }
    }
    while (!lambda.empty() && lambda.back() == 0) 
        lambda.pop_back();
    return lambda;
}

std::vector<int> RS_Decoder::chien_search(const std::vector<int>& lambda, std::vector<int>& error_positions){
    error_positions.clear();
    std::vector<int> X;
    for (int i = 0; i < n; i++) {
        int x = gf.div(1, gf.get_alpha_to()[i]);
        if (poly_eval(lambda, x) == 0) {
            error_positions.push_back(i);
            X.push_back(x);             
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
        corrected[pos] = GF_SUB(corrected[pos], error_values[idx]);
    }

    return corrected;
}

