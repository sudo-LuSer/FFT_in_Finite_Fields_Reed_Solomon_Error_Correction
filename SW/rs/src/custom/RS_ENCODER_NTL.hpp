#ifndef RS_ENCODER_NTL_HPP
#define RS_ENCODER_NTL_HPP

#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/GF2X.h>
#include <vector>
#include <stdexcept>

class RS_Encoder_NTL {
private:
    int n_;   // longueur du code
    int k_;   // taille du message
    int t_;   // capacité de correction
    int m_;   // degré du corps GF(2^m)

    NTL::GF2EX generator_;  // polynôme générateur

public:
    RS_Encoder_NTL(int n, int k);

    std::vector<int> encode(const std::vector<int>& message) const;

private:
    void build_generator_polynomial();

    NTL::GF2E intToGF2E(int value) const;
    int gf2eToInt(const NTL::GF2E& a) const;
};

#endif