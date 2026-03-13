#include "RS_Encoder.hpp"
#include "RS_tools.hpp"

RS_Encoder::RS_Encoder(int n, int k) : n(n), k(k) {
    if (n <= k) {
        throw std :: invalid_argument("n must be greater than k");
    }
    t = (n - k) / 2; 
    generator.clear();
}

void RS_Encoder::build_generator(GaloisField &gf) {
    generator.clear();
    generator.push_back(1);
    const std :: vector <int> alpha_to_reg = gf.get_alpha_to();
    for (int i = 1; i <= n-k; i++) {
        int alpha_i = alpha_to_reg[i];
        generator = poly_mult_by_binomial(generator, alpha_i, gf);
    }
}

std :: vector<int> RS_Encoder::poly_mult_by_binomial(const std :: vector<int>& poly, int a, GaloisField &gf) {
    size_t degree = poly.size();
    std :: vector<int> result(degree + 1, 0);
    
    for (size_t j = 0; j < degree; j++) {
        result[j + 1] = poly[j];
    }

    for (size_t j = 0; j < degree; j++) {
        result[j] = poly[j] != 0 ? gf.add(result[j], gf.mul(poly[j], a)) : result[j];
    }
    return result;
}

std :: vector<int> RS_Encoder::poly_div(const std :: vector<int>& dividend, const std :: vector<int>& divisor, GaloisField &gf) {
    std :: vector<int> remainder = dividend;
    int deg_d = (int)divisor.size() - 1;
    
    if (divisor.empty() || divisor[deg_d] == 0) {
        throw std :: invalid_argument("Divisor polynomial is zero or invalid");
    }
    
    while(remainder.size() >= divisor.size() && !remainder.empty()) {
        int deg_r = (int)remainder.size() - 1;
        int shift = deg_r - deg_d;
        
        int coef = gf.div(remainder[deg_r], divisor[deg_d]);
        
        for(int i = 0; i <= deg_d; i++) {
            if (i + shift < (int)remainder.size()) {
                remainder[i + shift] = gf.sub(remainder[i + shift], gf.mul(coef, divisor[i]));
            }
        }
        
        while(!remainder.empty() && remainder.back() == 0) {
            remainder.pop_back();
        }
    }
    
    if (remainder.empty()) {
        return {0};
    }
    
    return remainder;
}

std :: vector<int> RS_Encoder::encode(const std :: vector<int>& message, GaloisField &gf) {
    if (message.size() != (size_t)k) {
        throw std :: invalid_argument("Message length must be k = " + std :: to_string(k));
    }
    
    std :: vector<int> mx_poly(message.begin(), message.end());
    mx_poly.insert(mx_poly.begin(), n-k, 0);
    std :: vector<int> remainder = poly_div(mx_poly, generator, gf);
    
    remainder.resize(n - k, 0);
    
    std :: vector<int> codeword = remainder;
    codeword.insert(codeword.end(), message.begin(), message.end());
    
    if (codeword.size() != (size_t)n) {
        throw std :: runtime_error("Codeword length incorrect");
    }
    
    return codeword;
}