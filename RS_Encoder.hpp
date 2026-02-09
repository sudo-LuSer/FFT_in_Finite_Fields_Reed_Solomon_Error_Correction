#ifndef RS_ENCODER_HPP
#define RS_ENCODER_HPP

#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

class GaloisField;

class RS_Encoder {
private:
    int n;              
    int k;          
    int t;             
    vector<int> generator; 
    
public: 
    RS_Encoder(int n, int k);
    
    const vector<int>& get_generator() const { return generator; }
    int get_degree() const { return generator.empty() ? 0 : (int)generator.size() - 1; }
    
    void build_generator(GaloisField &gf);
    vector<int> encode(const vector<int>& message, GaloisField &gf);
    vector<int> poly_div(const vector<int>& dividend, const vector<int>& divisor, GaloisField &gf);
    vector<int> poly_mult_by_binomial(const vector<int>& poly, int a, GaloisField &gf);
};

#endif