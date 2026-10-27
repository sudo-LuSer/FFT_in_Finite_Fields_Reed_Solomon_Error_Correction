#pragma once
#ifndef RS_DECODER_HPP
#define RS_DECODER_HPP
#include <vector>
#include "RS_tools.hpp"

// Algebraic decoder over GF(2^m). Codeword symbols are coefficients of
// x^i in ascending order. first_root=1 uses alpha^1,...; first_root=0
// matches the alternate NTL encoder convention alpha^0,...
class RS_Decoder {
private:
    int n_, k_, redundancy_, t_, first_root_;
    GaloisField& gf_;
    std::vector<int> syndrome_roots_, inverse_powers_;
    bool fft_supported_ = false;
    int fft_cost_units_ = 0;
public:
    RS_Decoder(int n, int k, GaloisField& gf, int first_root = 1);
    std::vector<int> compute_syndromes(const std::vector<int>& received);
    std::vector<int> berlekamp_massey(const std::vector<int>& syndromes);
    std::vector<int> chien_search(const std::vector<int>& lambda,
                                  std::vector<int>& error_positions);
    std::vector<int> forney(const std::vector<int>& lambda,
                            const std::vector<int>& omega,
                            const std::vector<int>& error_positions,
                            const std::vector<int>& X);
    std::vector<int> poly_mult(const std::vector<int>& a, const std::vector<int>& b);
    int poly_eval(const std::vector<int>& poly, int x);

    // false means correction could not be verified; corrected remains unchanged.
    bool try_decode(const std::vector<int>& received, std::vector<int>& corrected);
    // Compatibility API: throws std::runtime_error if correction fails.
    std::vector<int> decode(const std::vector<int>& received);
};
#endif
