#include <iostream>
#include <vector>
#include <NTL/GF2E.h>
#include <NTL/GF2X.h>
#include "RS_Encoder_NTL.hpp"
#include "RS_tools.hpp" 
#include "RS_Encoder.hpp"
int main() {
    std :: cout << std :: endl;
    std :: cout << "----------------------------- Test RS NTT(7, 3) code -----------------------------";
    std :: cout << std :: endl;
    int m = 3;
    int n = 7;
    int k = 3;

    // Initialise NTL field using the same primitive polynomial as GaloisField
    uint32_t mask = poly_masks[m];
    NTL::GF2X prim;
    for (int i = 0; i <= m; ++i) {
        if (mask & (1 << i)) {
            NTL::SetCoeff(prim, i, 1);
        }
    }
    NTL::GF2E::init(prim);

    RS_Encoder_NTL encoder_ntt(n, k);

    std::vector<int> message(k);
    for (int i = 1; i <= k; ++i)
        message[i-1] = i; // Example message: [1, 2, 3]

    std::vector<int> codeword = encoder_ntt.encode(message);

    std :: cout << "Code: ";
    for (int c : codeword) {
        std :: cout << c << " ";
    }
    std :: cout << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "----------------------------- Test RS(7, 3) code -----------------------------";
    std :: cout << std :: endl;
    int t = (n - k) / 2; // Error correction capability
    // RS(7, 3) code parameters
    std::cout << "RS(" << n << ", " << k << ") code with error correction capability t = " << t << std::endl;
    // Create encoder and decoder
    GaloisField GF(m); // Create Galois Field GF(2)
    RS_Encoder encoder(n, k, GF);
    // Example message
    std::cout << "Original message: ";
    for (int m : message) {
        std::cout << m << " ";
    }
    std::cout << std::endl;
    // Encode the message
    encoder.encode(message, codeword);
    std::cout << "Encoded codeword: ";
    for (int c : codeword) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    // Introduce errors in the codeword
    std::vector<int> received = codeword;
    received[1] = 0; // Introduce an error
    if (t > 1) {
        received[2] = 0; // Introduce another error if t > 1
    }
    std::cout << "Received codeword with error: ";
    for (int r : received) {
        std::cout << r << " ";
    }

    std::cout << std::endl; 
}