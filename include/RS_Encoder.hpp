#pragma once
#ifndef RS_ENCODER_H
#define RS_ENCODER_H

#include <vector>
#include "RS_tools.hpp"

class RS_Encoder : public GaloisField {
private:
    int n, k, r, t;

    std::vector<int> generator;
    std::vector<int> parity;
    std::vector<int> mul_table;
public:
    RS_Encoder(int n, int k, const GaloisField& gf);

    void encode(const std::vector<int>& message,
                std::vector<int>& codeword);
};

#endif