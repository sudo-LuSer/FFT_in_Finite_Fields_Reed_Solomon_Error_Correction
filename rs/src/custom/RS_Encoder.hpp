#pragma once

#ifndef RS_ENCODER_HPP
#define RS_ENCODER_HPP
#define RS_ENCODER_HPP
#include <vector>

class GaloisField;

class RS_Encoder {
private:
    int n;              
    int k;          
    int t;             
    std :: vector<int> generator;
    GaloisField *gf; 

    std :: vector<int> mul_table; 
public: 
    RS_Encoder(int n, int k, GaloisField &gf);
    
    const std :: vector<int>& get_generator() const { return generator; }
    int get_degree() const { return generator.empty() ? 0 : (int)generator.size() - 1; }

    void encode(const std::vector<int>& message, std :: vector <int> &codeword);
};

#endif