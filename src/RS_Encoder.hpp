#ifndef RS_ENCODER_HPP
#define RS_ENCODER_HPP

#include <vector>
#include "RS_tools.hpp"

class RS_Encoder {
private:
    const int n;
    const int k;
    const int n_rdncy;

    int field_order;

    std::vector<int> generator;
    std::vector<int> g_log;     // generator coefficients in log form for fast LFSR
    std::vector<int> parity;    // pre-allocated parity buffer, reused every encode()

public:
    RS_Encoder(int n, int k);

    const std::vector<int>& get_generator() const { return generator; }
    int get_degree() const { return n_rdncy; }

    void build_generator(GaloisField& gf);
    std::vector<int> encode(const std::vector<int>& message, GaloisField& gf);
};

#endif