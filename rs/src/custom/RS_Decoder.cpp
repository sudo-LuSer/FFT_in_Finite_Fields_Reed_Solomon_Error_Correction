#include "RS_Decoder.hpp"

RS_Decoder::RS_Decoder(int n, int k, GaloisField &gf)
    : n(n), k(k), gf(gf), BerlekampMassey(gf) {
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
            X.push_back(gf.inv(gf.get_alpha_to()[i]));
        }

        for (int j = 1; j <= L; j++)
            reg[j] = gf.mul(reg[j], gf.inv(gf.get_alpha_to()[j]));
    }

    return X;
}

std::vector<int> formal_derivative(const std::vector<int>& poly) {
    if (poly.size() <= 1) return {0};
    std::vector<int> deriv(poly.size() - 1, 0);
    for (size_t i = 1; i < poly.size(); i += 2)
        deriv[i - 1] = poly[i];
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
    for (int s : syndromes) if (s != 0) { all_zero = false; break; }
    if (all_zero) return received;

    set_source(syndromes);
    std::vector<int> lambda = compute_coefficients();

    std::vector<int> omega = poly_mult(syndromes, lambda);
    omega.resize(2 * t);

    std::vector<int> error_positions;
    std::vector<int> X = chien_search(lambda, error_positions);

    // if (error_positions.size() != lambda.size() - 1) {
    //     throw std::runtime_error("Échec du décodage : nombre d'erreurs incohérent");
    // }

    std::vector<int> error_values = forney(lambda, omega, error_positions, X);
    std::vector<int> corrected = received;
    for (size_t i = 0; i < error_positions.size(); ++i) {
        int pos = error_positions[i];  
        corrected[pos] = GF_SUB(corrected[pos], error_values[i]);
    }
    return corrected;
}



// my_utility functions
inline std::vector<int> RS_Decoder :: poly_mult(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> res(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] == 0) continue;
        for (size_t j = 0; j < b.size(); ++j) {
            if (b[j] == 0) continue;
            res[i+j] = GF_ADD(res[i+j], gf.mul(a[i], b[j]));
        }
    }
    return res;
}
inline int RS_Decoder :: poly_eval(const std::vector<int>& poly, int x) {
    int result = 0;
    for (int i = poly.size() - 1; i >= 0; --i) {
        result = GF_ADD(gf.mul(result, x), poly[i]);
    }
    return result;
}