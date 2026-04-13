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

public:
    RS_Decoder(int n, int k, GaloisField &gf);
    std::vector<int> compute_syndromes(const std::vector<int>& received);
    std::vector<int> berlekamp_massey(const std::vector<int>& syndromes);
    std::vector<int> chien_search(const std::vector<int>& lambda, std::vector<int>& error_positions);
    std::vector<int> forney(const std::vector<int>& lambda, const std::vector<int>& omega, const std::vector<int>& error_positions, const std::vector<int>& X);
    std::vector<int> decode(const std::vector<int>& received);
    inline std::vector<int> poly_mult(const std::vector<int>& a, const std::vector<int>& b) {
        std::vector<int> res(a.size() + b.size() - 1, 0);
        for (size_t i = 0; i < a.size(); ++i) {
             if (a[i] == 0) continue;
            for (size_t j = 0; j < b.size(); ++j) {
                if (b[j] == 0) continue;
                res[i+j] = GF_ADD(res[i+j], gf.mul(a[i], b[j]));
            }
        }
        return res;
    }
    inline int poly_eval(const std::vector<int>& poly, int x) {
        int result = 0;
        for (int i = poly.size() - 1; i >= 0; --i) {
            result = GF_ADD(gf.mul(result, x), poly[i]);
        }
        return result;
    }
};

#endif