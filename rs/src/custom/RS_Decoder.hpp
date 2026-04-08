#pragma once

#ifndef RS_DECODER_HPP
#define RS_DECODER_HPP
#include <vector>
#include "RS_tools.hpp"
#include "RS_Encoder.hpp"

class RS_Decoder {
private:
    int n, k, t;
    GaloisField &gf;
    bool error_flag;
public:
    RS_Decoder(int n, int k, GaloisField &gf);
    std::vector<int> compute_syndromes(const std::vector<int>& received);
    void berlekamp_massey(const std::vector<int>& s, std::vector<int>& lambda_out);
    std::vector<int> chien_search(const std::vector<int>& lambda, std::vector<int>& error_positions);
    std::vector<int> forney(const std::vector<int>& lambda, const std::vector<int>& omega, const std::vector<int>& error_positions, const std::vector<int>& X);
    std::vector<int> decode(const std::vector<int>& received);
    std::vector<int> poly_mult(const std::vector<int>& a, const std::vector<int>& b);
    int poly_eval(const std::vector<int>& poly, int x);
    std :: vector<int> formal_derivative(const std::vector<int>& poly);
protected: 
    std :: vector <int> AlphaPow_reg;
    std :: vector <int> AlphaInv_reg;
    std::  vector<int> alpha_pow_syn;
    std::  vector<int> alpha_inv;
    std::  vector<int> syndrome_table;
    std::  vector<int> lambda_scratch;
    std::  vector<int> omega_scratch;
    std::  vector<int> error_positions_scratch;
    std::  vector<int> X_scratch; 
};

#endif