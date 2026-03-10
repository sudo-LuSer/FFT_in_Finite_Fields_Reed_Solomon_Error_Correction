#include "RS_Encoder.hpp"
#include <stdexcept>
#include <algorithm>

// ─────────────────────────────────────────────
//  Constructor — same signature as before
// ─────────────────────────────────────────────
RS_Encoder::RS_Encoder(int n, int k)
    : n(n), k(k), n_rdncy(n - k), field_order(0)
{
    if (n <= k)
        throw std::invalid_argument("n must be greater than k");
}

// ─────────────────────────────────────────────
//  build_generator — same signature as before
//  Internally: in-place multiply + g_log precompute
//  + parity buffer pre-allocation
// ─────────────────────────────────────────────
void RS_Encoder::build_generator(GaloisField& gf) {
    field_order = gf.get_size() - 1;
    const int* alpha_to = gf.get_alpha_to().data();
    const int* index_of = gf.get_index_of().data();

    generator.assign(n_rdncy + 1, 0);
    generator[0] = 1;

    // In-place multiply by (X + alpha^i), high to low to avoid overwrite
    for (int i = 1; i <= n_rdncy; i++) {
        int alpha_i = alpha_to[i % field_order];
        for (int j = i; j > 0; j--) {
            if (generator[j - 1] != 0)
                generator[j] ^= gf.mul(generator[j - 1], alpha_i);
        }
        generator[0] = gf.mul(generator[0], alpha_i);
    }

    // Pre-compute log form — -1 sentinel for zero coefficients
    g_log.resize(n_rdncy + 1);
    for (int i = 0; i <= n_rdncy; i++)
        g_log[i] = (generator[i] != 0) ? index_of[generator[i]] : -1;

    // Pre-allocate parity buffer once here, reused in every encode() call
    parity.assign(n_rdncy, 0);
}

// ─────────────────────────────────────────────
//  encode — same signature as before
//  Internally: LFSR instead of poly_div,
//  raw pointer access, no per-call allocation
// ─────────────────────────────────────────────
std::vector<int> RS_Encoder::encode(const std::vector<int>& message, GaloisField& gf) {
    if ((int)message.size() != k)
        throw std::invalid_argument("Message length must be k = " + std::to_string(k));

    const int* alpha_to = gf.get_alpha_to().data();
    const int* index_of = gf.get_index_of().data();

    // Reset parity buffer — no heap allocation
    std::fill(parity.begin(), parity.end(), 0);

    // LFSR: process message symbols high→low
    for (int i = k - 1; i >= 0; i--) {
        const int feedback_sym = message[i] ^ parity[n_rdncy - 1];
        const int log_fb = (feedback_sym != 0) ? index_of[feedback_sym] : -1;

        if (log_fb != -1) {
            for (int j = n_rdncy - 1; j > 0; j--) {
                parity[j] = parity[j - 1];
                if (g_log[j] != -1)
                    parity[j] ^= alpha_to[(g_log[j] + log_fb) % field_order];
            }
            parity[0] = (g_log[0] != -1) ? alpha_to[(g_log[0] + log_fb) % field_order] : 0;
        } else {
            for (int j = n_rdncy - 1; j > 0; j--)
                parity[j] = parity[j - 1];
            parity[0] = 0;
        }
    }

    // Build codeword: [parity | message] — one reserve, two inserts, no copies
    std::vector<int> codeword;
    codeword.reserve(n);
    codeword.insert(codeword.end(), parity.begin(), parity.end());
    codeword.insert(codeword.end(), message.begin(), message.end());

    return codeword;
}