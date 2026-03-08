#ifndef RS_ENCODER_HPP
#define RS_ENCODER_HPP

#include <vector>

class GaloisField;

class RS_Encoder {
private:
    int n;              
    int k;          
    int t;             
    std :: vector<int> generator; 
    
public: 
    RS_Encoder(int n, int k);
    
    const std :: vector<int>& get_generator() const { return generator; }
    int get_degree() const { return generator.empty() ? 0 : (int)generator.size() - 1; }
    
    void build_generator(GaloisField &gf);
    std :: vector<int> encode(const std :: vector<int>& message, GaloisField &gf);
    std :: vector<int> poly_div(const std :: vector<int>& dividend, const std :: vector<int>& divisor, GaloisField &gf);
    std :: vector<int> poly_mult_by_binomial(const std :: vector<int>& poly, int a, GaloisField &gf);
};

#endif