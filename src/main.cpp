#include <iostream>
#include <vector>
#include <NTL/GF2E.h>
#include <NTL/GF2X.h>
#include "RS_Encoder_NTL.hpp"
#include "RS_tools.hpp" 
#include "RS_Encoder.hpp"
#include "RS_Decoder.hpp"
// #include "RS_DECODER_NTL.hpp"
int main() {
    std :: cout << std :: endl;
    std :: cout << "----------------------------- Test RS NTT(7, 3) code -----------------------------";
    std :: cout << std :: endl;
    int m = 3;
    int n = 7;
    int k = 3;
    std::vector<int> message = {3 , 2 , 1}; // Example message of length k
    std::vector<int> codeword(n); // To hold the encoded codeword
    std::vector<int> received(n); // To hold the received codeword with errors
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
    received = codeword;
    received[1] = 0; // Introduce an error
    if (t > 1) {
        received[2] = 0; // Introduce another error if t > 1
    }
    std::cout << "Received codeword with error: ";
    for (int r : received) {
        std::cout << r << " ";
    }

    RS_Decoder decoder(n, k, GF);
    std::vector<int> decoded_message(k); // To hold the decoded message
    decoded_message = decoder.decode(received);
    std::cout << "Decoded message: ";
    for (int d : decoded_message) {
        std::cout << d << " ";
    }


    std::cout << std::endl; 
}