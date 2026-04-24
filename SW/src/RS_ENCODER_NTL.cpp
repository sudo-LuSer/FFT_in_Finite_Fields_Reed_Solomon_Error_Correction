#include "RS_Encoder_NTL.hpp"
#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/GF2X.h>
#include <vector>
#include <stdexcept>

RS_Encoder_NTL::RS_Encoder_NTL(int n, int k) : n_(n), k_(k) {
    if (n_ <= k_)
        throw std::invalid_argument("n must be strictly greater than k.");

    t_ = (n_ - k_) / 2;

    m_ = NTL::deg(NTL::GF2E::modulus());

    build_generator_polynomial();
}

void RS_Encoder_NTL::build_generator_polynomial(){
    clear(generator_);
    SetCoeff(generator_, 0, 1);

    NTL::GF2X poly_x;
    SetCoeff(poly_x, 1);

    NTL::GF2E alpha;
    conv(alpha, poly_x);

    for (int i = 0; i < n_ - k_; ++i) {
        NTL::GF2EX factor;
        SetCoeff(factor, 1, 1);

        NTL::GF2E root = power(alpha, i);
        SetCoeff(factor, 0, root);

        generator_ *= factor;
    }
}


NTL::GF2E RS_Encoder_NTL::intToGF2E(int value) const {
    if (value < 0) {
        throw std::invalid_argument("Les symboles doivent être non négatifs.");
    }

    // On suppose que le symbole est codé sur m bits.
    // Limite pratique pour rester compatible avec int.
    if (m_ > 30) {
        throw std::runtime_error("m trop grand pour une conversion sûre vers int.");
    }

    const int max_symbol = (1 << m_) - 1;
    if (value > max_symbol) {
        throw std::invalid_argument("Le symbole est hors de l'intervalle [0, 2^m - 1].");
    }

    NTL::GF2X poly;
    for (int b = 0; b < m_; ++b) {
        if ((value >> b) & 1) {
            NTL::SetCoeff(poly, b, 1);
        }
    }

    NTL::GF2E out;
    NTL::conv(out, poly);
    return out;
}

int RS_Encoder_NTL::gf2eToInt(const NTL::GF2E& a) const {
    NTL::GF2X poly = NTL::rep(a);

    unsigned long long value = 0;
    long d = NTL::deg(poly);
    if (d >= 0) {
        for (long b = 0; b <= d; ++b) {
            if (NTL::coeff(poly, b) != 0) {
                value |= (1ULL << b);
            }
        }
    }

    return static_cast<int>(value);
}

std::vector<int> RS_Encoder_NTL::encode(const std::vector<int>& message) const {
    if (message.size() != static_cast<size_t>(k_)) {
        throw std::invalid_argument("La taille du message doit être exactement k.");
    }

    NTL::GF2EX message_poly;
    for (int i = 0; i < k_; ++i) {
        NTL::GF2E c = intToGF2E(message[i]);
        NTL::SetCoeff(message_poly, i, c);
    }

    NTL::GF2EX shifted;
    NTL::LeftShift(shifted, message_poly, n_ - k_);

    NTL::GF2EX remainder = shifted % generator_;

    NTL::GF2EX codeword_poly = shifted + remainder;

    std::vector<int> codeword(n_, 0);
    for (int i = 0; i < n_; ++i) {
        NTL::GF2E c = NTL::coeff(codeword_poly, i);
        codeword[i] = gf2eToInt(c);
    }

    return codeword;
}