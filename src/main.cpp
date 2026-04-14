#include <iostream>
#include <vector>
#include "RS_tools.hpp"
#include "RS_Encoder.hpp"
#include "RS_Decoder.hpp"

int main() {
    int m = 8;
    int n = 255;
    int k = 239;
    int t = (n - k) / 2;

    std::cout << "RS(" << n << ", " << k << ") code, t = " << t << std::endl;

    GaloisField gf(m);
    RS_Encoder encoder(n, k, gf);
    RS_Decoder decoder(n, k, gf);

    // Message aléatoire
    std::vector<int> message(k);
    for (int i = 0; i < k; ++i) {
        message[i] = i+1; // Juste pour avoir des valeurs non nulles
    }

    std :: cout << "Message original : " << std::endl;
    for(auto x : message) std::cout << x << " ";
    std::cout << std::endl;
    
    std::vector<int> codeword;
    encoder.encode(message, codeword);

std::cout << "Codeword encodé : " << std::endl;

    for(int i = 0; i < n; ++i) {
        std::cout << codeword[i] << " ";
    }

    std :: cout << std::endl;

    // Ajout d'erreurs
    std::vector<int> received = codeword;
    received[0 + (n - k)] ^= 0xFF;
    received[1 + (n - k)] ^= 0x55;

    std :: cout << "Message reçu (avec erreurs) : " << std::endl;
    for(auto x : received) std::cout << x << " ";
    std::cout << std::endl;

    // Décodage
    std::vector<int> decoded= decoder.decode(received);
    std :: cout << "Message décodé : " << std :: endl ;
    for(auto x : decoded) std::cout << x << " ";
    std::cout << std::endl;
}