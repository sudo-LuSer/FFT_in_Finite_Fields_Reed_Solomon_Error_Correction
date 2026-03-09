#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include "RS_Decoder.hpp"
#include <iostream>
#include <chrono>

#define pb push_back

int main(){
    GaloisField gf(8);
    std :: vector <int> messages;
    int n = 255, k = 239; 
    for(int i = 1;i <= k; i++)
        messages.pb(gf.get_alpha_to()[gf.get_index_of()[i]]);
    auto start_total = std::chrono::high_resolution_clock::now();
    RS_Encoder RS_Enc(n,k);
    RS_Enc.build_generator(gf);
    std :: vector<int> gen = RS_Enc.get_generator();
    std :: cout << "Generator polynomial coefficients (from highest degree to constant): ";
    for (int i = gen.size() - 1; i >= 0; i--) {
        std :: cout << gen[i] << " ";
    }
    std :: cout << std :: endl;
    std :: cout << std :: endl; 
    std :: vector<int> mx_poly(n, 0);
    for (int i = 0; i < k; i++) {
        mx_poly[n - k + i] = messages[i];
    }
    auto start_div = std::chrono::high_resolution_clock::now();
    std :: vector<int> remainder = RS_Enc.poly_div(mx_poly, gen, gf);
    auto end_div = std::chrono::high_resolution_clock::now();
    for (int i = remainder.size() - 1; i >= 0; i--) {
        std :: cout << remainder[i] << " ";
    }
    std :: cout << std :: endl; 
    std :: cout << std :: endl;
    auto start_encode = std::chrono::high_resolution_clock::now();
    std :: vector <int> CodeWord = RS_Enc.encode(messages, gf); 
    auto end_encode = std::chrono::high_resolution_clock::now();
    // Encoder
    std :: cout << "Codeword: ";
    for(auto x : CodeWord)
        std :: cout << x << " ";
    std :: cout << std :: endl;
    std :: cout << std :: endl; 

    /// injection d'erreur 

    // CodeWord[0] = gf.add(CodeWord[0], 1); 
    // CodeWord[1] = gf.add(CodeWord[1], 1); 
    // CodeWord[2] = gf.add(CodeWord[2], 1);
    // CodeWord[3] = gf.add(CodeWord[3], 1); 
    // CodeWord[4] = gf.add(CodeWord[4], 1); 
    // CodeWord[5] = gf.add(CodeWord[5], 1);
    // CodeWord[6] = gf.add(CodeWord[6], 1); 
    // CodeWord[7] = gf.add(CodeWord[7], 1);
    //CodeWord[8] = gf.add(CodeWord[8], 1);

    // Decodage

    std :: cout << "Sortie du décodeur :"; 

    RS_Decoder RS_Dec(n, k, gf); 
    auto start_decode = std::chrono::high_resolution_clock::now();
    std :: vector <int> ERR_Correc = RS_Dec.decode(CodeWord);
    auto end_decode = std::chrono::high_resolution_clock::now();
    
    auto end_total = std::chrono::high_resolution_clock::now();
    
    for(auto x : ERR_Correc)
        std :: cout << x << " "; 
    
    std :: cout << std :: endl; 

    auto duration_div = std::chrono::duration_cast<std::chrono::microseconds>(end_div - start_div);
    auto duration_encode = std::chrono::duration_cast<std::chrono::microseconds>(end_encode - start_encode);
    auto duration_decode = std::chrono::duration_cast<std::chrono::microseconds>(end_decode - start_decode);
    auto duration_total = std::chrono::duration_cast<std::chrono::microseconds>(end_total - start_total);
    
    std::cout << "\n--- Temps de calcul ---" << std::endl;
    std::cout << "Division polynomiale (reste) : " << duration_div.count() << " micros" << std::endl;
    std::cout << "Encodage complet : " << duration_encode.count() << " micros" << std::endl;
    std::cout << "Décodage : " << duration_decode.count() << " micros" << std::endl;
    std::cout << "Temps total : " << duration_total.count() << " micros" << std::endl;
}