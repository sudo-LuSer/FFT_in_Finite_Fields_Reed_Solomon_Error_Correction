#ifndef RS_ENCODER_HPP
#define RS_ENCODER_HPP

#include "RS_tools.hpp"

class RS_Encoder {
private:
    int n;              
    int k;          
    int t;             
    vector<int> generator; 
    
public: 

    RS_Encoder(int n, int k) : n(n), k(k) {
        t = (n - k) / 2;
        if (n > 255) {
            throw invalid_argument("RS codes over GF(256) have max length 255");
        }
    }
    
    const vector<int>& get_generator() const { return generator; }
    int get_degree() const { return generator.empty() ? 0 : generator.size() - 1; }
    
    void build_generator(GaloisField &gf);
    vector<int> encode(const vector<int>& message, GaloisField &gf);
    vector<int> poly_div(const vector<int>& dividend, const vector<int>& divisor, GaloisField &gf);
    vector<int> poly_mult_by_binomial(const vector<int>& poly, int a, GaloisField &gf);
    
};

#endif