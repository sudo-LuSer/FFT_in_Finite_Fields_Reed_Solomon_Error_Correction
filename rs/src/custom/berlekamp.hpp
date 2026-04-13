#pragma once
#include <vector>
#include "RS_tools.hpp"

class BerlekampMassey {
protected:
    std::vector<int> source;
    const GaloisField& gf;

public:
    BerlekampMassey(const GaloisField& gf) : gf(gf) {}
    void set_source(const std::vector<int>& src) { source = src; }
    std::vector<int> compute_coefficients();
};