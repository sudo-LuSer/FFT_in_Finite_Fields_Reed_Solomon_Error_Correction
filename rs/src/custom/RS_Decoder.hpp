#pragma once
#include <vector>
#include "RS_tools.hpp"
#include "berlekamp.hpp"

class RS_Decoder : public BerlekampMassey {
private:
    int n, k, t;
    GaloisField &gf;

public:
    RS_Decoder(int n, int k, GaloisField &gf);
    std::vector<int> compute_syndromes(const std::vector<int>& received);
    std::vector<int> chien_search(const std::vector<int>& lambda, std::vector<int>& error_positions);
    std::vector<int> forney(const std::vector<int>& lambda, const std::vector<int>& omega,
                            const std::vector<int>& error_positions, const std::vector<int>& X);
    std::vector<int> decode(const std::vector<int>& received);

    // my_utility functions
    inline std::vector<int> poly_mult(const std::vector<int>& a, const std::vector<int>& b);
    inline int poly_eval(const std::vector<int>& poly, int x);
};