#include "RS_Encoder.hpp"
#include "RS_tools.hpp"

#include <algorithm>

RS_Encoder::RS_Encoder(int n, int k, GaloisField &gf) : n(n), k(k), gf(&gf){
    if (n <= k)
        throw std::invalid_argument("n must be greater than k");

    t = (n - k) / 2;

    generator.push_back(1);

    // à faire : init de la variable start_deg avec une LOOK UP TABLE

    int start_deg = 1;

    for (int i = start_deg; i < start_deg + (n - k); i++){
        int alpha_i = gf.get_alpha_to()[i];
        generator = poly_mult_by_binomial(generator, alpha_i);
    }

    // std::vector<int> coeffs_index = {136, 15, 30, 26, 23, 29, 224, 53, 125, 36, 190, 233, 210, 196, 183, 151, 0};
    // generator.clear();
    // for (int idx : coeffs_index) {
    //     if (idx == -1)
    //         generator.push_back(0);
    //     else
    //         generator.push_back(gf.get_alpha_to()[idx]);
    // }
}

std::vector<int> RS_Encoder::poly_mult_by_binomial(const std::vector<int>& poly, int a)
{
    std::vector<int> result(poly.size() + 1, 0);

    result[0] = gf->mul(poly[0], a);

    for(size_t i = 1; i < poly.size(); i++)
        result[i] = gf->add(gf->mul(poly[i], a), poly[i-1]);

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

std::vector<int> RS_Encoder::encode(const std::vector<int>& message){
    if (message.size() != (size_t)k)
        throw std::invalid_argument("Message length must be k = " + std::to_string(k));

    int r = n - k;
    std::vector<int> parity(r, 0);

    for (int i = k - 1; i >= 0; i--){
        int feedback = gf->add(message[i], parity[r - 1]);
        for (int j = r - 1; j > 0; j--){
                parity[j] = gf->add(parity[j - 1], gf->mul(feedback, generator[j]));
        }
        parity[0] = gf->mul(feedback, generator[0]);
    }

    std::vector<int> codeword = parity;
    codeword.insert(codeword.end(), message.begin(), message.end());
    return codeword;
}