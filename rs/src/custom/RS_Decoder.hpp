#pragma once
#ifndef RS_DECODER_H
#define RS_DECODER_H

#include <vector>
#include "RS_tools.hpp"

class RS_Decoder {
private:
    int n, k, t;
    const GaloisField& gf;

    std::vector<int> syndrome;
    std::vector<int> lambda;
    std::vector<int> B;

public:
    RS_Decoder(int n, int k, const GaloisField& gf);

    std::vector<int> decode(const std::vector<int>& received);

private:
    void compute_syndrome(const std::vector<int>& r);
    void berlekamp_massey();
};

#endif