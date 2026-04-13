#include "berlekamp.hpp"

std::vector<int> BerlekampMassey::compute_coefficients() {
    int L = 0;
    std::vector<int> lambda(1, 1);   // Λ(x)
    std::vector<int> b(1, 1);        // B(x) = polynôme correcteur précédent
    int m = 1;                       // indice d'itération

    for (int r = 0; r < (int)source.size(); ++r) {
        // Calcul du delta
        int delta = source[r];
        for (int j = 1; j <= L; ++j) {
            delta = GF_ADD(delta, gf.mul(lambda[j], source[r - j]));
        }

        if (delta != 0) {
            std::vector<int> old_lambda = lambda;
            // λ(x) ← λ(x) + delta * B(x) * x^m
            if (lambda.size() < b.size() + m) {
                lambda.resize(b.size() + m, 0);
            }
            for (size_t i = 0; i < b.size(); ++i) {
                lambda[i + m] = GF_ADD(lambda[i + m], gf.mul(delta, b[i]));
            }

            if (2 * L <= r) {
                L = r + 1 - L;
                b = old_lambda;
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
    // Normaliser pour que Λ(0) = 1
    lambda.resize(L + 1);
    return lambda;
}