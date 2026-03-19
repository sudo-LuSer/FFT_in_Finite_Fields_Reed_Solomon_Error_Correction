#include "RS_Encoder.hpp"
#include "RS_tools.hpp"

// #include <aff3ct.hpp>
// #include <algorithm>
// #include <immintrin.h>

RS_Encoder::RS_Encoder(int n, int k, GaloisField &gf) : n(n), k(k), gf(&gf) {
    if (n <= k)
        throw std::invalid_argument("n must be greater than k");

    t = (n - k) / 2;

    // aff3ct::tools :: RS_polynomial_generator rs_g(n,t);
    // generator = rs_g.get_g();

    generator.push_back(1);

    int start_deg = 1;   //remplacer par une LUT si nécessaire

    for (int i = 1; i <= (n - k); i++) {
        int alpha_i = gf.get_alpha_to()[i];
        generator = poly_mult_by_binomial(generator, alpha_i);
    }

    int r = n - k;

    mul_table.resize(r * (n + 1));

    for (int j = 0; j < r; j++) {
        for (int x = 0; x < n + 1; x++) {
            mul_table[j * (n + 1) + x] = gf.mul(x, generator[j]);
        }
    }
}

std::vector<int> RS_Encoder::poly_mult_by_binomial(const std::vector<int>& poly, int a)
{
    std::vector<int> result(poly.size() + 1, 0);

    result[0] = gf->mul(poly[0], a);

    for(size_t i = 1; i < poly.size(); i++)
        result[i] = GF_ADD(gf->mul(poly[i], a), poly[i-1]);

    result[poly.size()] = poly.back();

    return result;
}

// std :: vector<int> RS_Encoder::poly_div(const std :: vector<int>& dividend, const std :: vector<int>& divisor) {
//     std :: vector<int> remainder = dividend;
//     int deg_d = (int)divisor.size() - 1;
    
//     if (divisor.empty() || divisor[deg_d] == 0) {
//         throw std :: invalid_argument("Divisor polynomial is zero or invalid");
//     }
    
//     while(remainder.size() >= divisor.size() && !remainder.empty()) {
//         int deg_r = (int)remainder.size() - 1;
//         int shift = deg_r - deg_d;
        
//         int coef = gf->div(remainder[deg_r], divisor[deg_d]);
        
//         for(int i = 0; i <= deg_d; i++) {
//             if (i + shift < (int)remainder.size()) {
//                 remainder[i + shift] = gf->sub(remainder[i + shift], gf->mul(coef, divisor[i]));
//             }
//         }
        
//         while(!remainder.empty() && remainder.back() == 0) {
//             remainder.pop_back();
//         }
//     }
    
//     if (remainder.empty()) {
//         return {0};
//     }
    
//     return remainder;
// }

void RS_Encoder::encode(const std::vector<int>& message, std :: vector <int> &codeword){
    if (message.size() != (size_t)k)
        throw std::invalid_argument("Message length must be k = " + std::to_string(k));

    int r = n - k;
    std::vector<int> parity(r, 0);

    for (int i = k - 1; i >= 0; i--){
        int feedback = GF_ADD(message[i], parity[r - 1]);

        for (int j = r - 1; j > 0; j--){
            parity[j] = GF_ADD(parity[j - 1], mul_table[j*(n+1) + feedback]);
        }

        parity[0] = mul_table[feedback];
    }

    codeword.resize(n);
    std::copy(parity.begin(), parity.end(), codeword.begin());
    std::copy(message.begin(), message.end(), codeword.begin() + r);
}