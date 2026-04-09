#include <iostream>
#include "RS_tools.hpp"
#include "RS_Encoder.hpp"
#include "RS_Decoder.hpp"

int main(){
    // RS(7, 3) code
    int n = 7; // codeword length       
    int k = 3; // message length
    int m = 3; // degree of the field GF(2^m) with m >= log2(n)
    int t = (n - k) / 2; // error correction capability
    std::cout << "RS(" << n << ", " << k << ") code with error correction capability t = " << t << std::endl;
    // Create encoder and decoder
    GaloisField GF(m); // Create Galois Field GF(2)
    RS_Encoder encoder(n, k, GF);
    RS_Decoder decoder(n, k, GF);
    // Example message
    std::vector<int> message = {3, 2, 1}; // Example message of length k
    std::cout << "Original message: ";
    for (int m : message) {
        std::cout << m << " ";
    }
    std::cout << std::endl;
    // Encode the message
    std::vector<int> codeword;
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
    // Decode the received codeword
    std::vector<int> decoded_message = decoder.decode(received);

    std::cout << "Decoded message: ";
    for (int d : decoded_message) {
        std::cout << d << " ";
    }   
}