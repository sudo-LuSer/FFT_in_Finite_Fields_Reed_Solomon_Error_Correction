#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include <string>

RS_Encoder::RS_Encoder(int n, int k) : n(n), k(k) {
    if (n <= k) {
        throw invalid_argument("n must be greater than k");
    }
    t = (n - k) / 2; 
    generator.clear();
}

void RS_Encoder::build_generator(GaloisField &gf) {
    generator.clear();
    generator.push_back(1);
    for (int i = 1; i <= n-k; i++) {
        int alpha_i = gf.get_alpha_to()[i % (gf.get_size() - 1)];
        generator = poly_mult_by_binomial(generator, alpha_i, gf);
    }
}

vector<int> RS_Encoder::poly_mult_by_binomial(const vector<int>& poly, int a, GaloisField &gf) {
    size_t degree = poly.size();
    vector<int> result(degree + 1, 0);
    
    for (size_t j = 0; j < degree; j++) {
        result[j + 1] = poly[j];
    }

    for (size_t j = 0; j < degree; j++) {
        if (poly[j] != 0) {
            result[j] = gf.add(result[j], gf.mul(poly[j], a));
        }
    }
    return result;
}

vector<int> RS_Encoder::poly_div(const vector<int>& dividend, const vector<int>& divisor, GaloisField &gf) {
    vector<int> remainder = dividend;
    int deg_d = (int)divisor.size() - 1;
    
    if (divisor.empty() || divisor[deg_d] == 0) {
        throw invalid_argument("Divisor polynomial is zero or invalid");
    }
    
    while(remainder.size() >= divisor.size() && !remainder.empty()) {
        int deg_r = (int)remainder.size() - 1;
        int shift = deg_r - deg_d;
        
        if (divisor[deg_d] == 0) {
            throw runtime_error("Leading coefficient of divisor is zero");
        }
        
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

vector<int> RS_Encoder::encode(const vector<int>& message, GaloisField &gf) {
    if (message.size() != (size_t)k) {
        throw invalid_argument("Message length must be k = " + to_string(k));
    }
    
    vector<int> mx_poly(n, 0);
    for (int i = 0; i < k; i++) {
        mx_poly[n - k + i] = message[i];
    }
    
    vector<int> remainder = poly_div(mx_poly, generator, gf);
    
    remainder.resize(n - k, 0);
    
    vector<int> codeword = remainder;
    codeword.insert(codeword.end(), message.begin(), message.end());
    
    if (codeword.size() != (size_t)n) {
        throw runtime_error("Codeword length incorrect");
    }
    
    return codeword;
}