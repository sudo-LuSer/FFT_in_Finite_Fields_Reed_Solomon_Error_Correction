#include <iostream>
#include <vector>
#include <cstdint>

#include "RS_tools.hpp"
#include "RS_Decoder.hpp"


std::vector<int> bits21_to_symbols(uint32_t data) {
    std::vector<int> symbols(7);
    for (int i = 0; i < 7; ++i)
        symbols[i] = (data >> (3 * i)) & 0x7;  
    return symbols;
}

uint8_t symbols_to_byte(const std::vector<int>& msg) {
    return (msg[2] << 6) | (msg[1] << 3) | msg[0];
}

// ------------------------------------------------------------------
// Point d'entrée
// ------------------------------------------------------------------
int main() {
    const int m = 3;
    const int n = (1 << m) - 1;   // 7
    const int k = 3;
    const int t = (n - k) / 2;    // 2

    GaloisField gf(m);
    gf.define_generator(n - k);   

    // Décodeur Reed-Solomon
    RS_Decoder decoder(n, k, gf);

    uint32_t received_data = 0x16ac51;  
    std::vector<int> received = bits21_to_symbols(received_data);

    std::cout << "Symbôles reçus : ";
    for (int s : received) std::cout << s << " ";
    std::cout << std::endl;

    std::vector<int> corrected = decoder.decode(received);

    std::cout << "Mot corrigé    : ";
    for (int s : corrected) std::cout << s << " ";
    std::cout << std::endl;

    std::vector<int> message(k);
    for (int i = 0; i < k; ++i)
        message[i] = corrected[i];

    std::cout << "Message (3 symb) : ";
    for (int s : message) std::cout << s << " ";
    std::cout << std::endl;

    uint8_t enigma_char = symbols_to_byte(message);
    std::cout << "Caractère Enigma décodé : 0x"
              << std::hex << (int)enigma_char << std::dec
              << " ('" << (char)enigma_char << "')" << std::endl;

    return 0;
}