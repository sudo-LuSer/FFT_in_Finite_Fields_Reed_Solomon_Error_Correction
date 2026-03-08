#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include "RS_Decoder.hpp"
#include <iostream>
int main(){
    GaloisField gf(3);
    std :: vector <int> messages;
    //messages.push_back(gf.get_alpha_to()[0]);  // 1
    messages.push_back(gf.get_alpha_to()[1]);  // 2
    messages.push_back(gf.get_alpha_to()[2]);  // 4
    messages.push_back(gf.get_alpha_to()[3]);  // 3
    int n = 7, k = 3; 
    RS_Encoder RS_Enc(n,k);
    RS_Enc.build_generator(gf);
    std :: vector<int> gen = RS_Enc.get_generator();
    std :: cout << "Generator polynomial coefficients (from highest degree to constant): ";
    for (int i = gen.size() - 1; i >= 0; i--) {
        std :: cout << gen[i] << " ";
    }
    std :: cout << std :: endl;
    std :: vector<int> mx_poly(n, 0);
    for (int i = 0; i < k; i++) {
        mx_poly[n - k + i] = messages[i];
    }
    
    std :: vector<int> remainder = RS_Enc.poly_div(mx_poly, gen, gf);

    for (int i = remainder.size() - 1; i >= 0; i--) {
        std :: cout << remainder[i] << " ";
    }
    std :: cout << std :: endl; 

    std :: vector <int> CodeWord = RS_Enc.encode(messages, gf); 
    // Encoder
    std :: cout << "Codeword: ";
    for(auto x : CodeWord)
        std :: cout << x << " ";
    std :: cout << std :: endl;

    /// injection d'erreur 

    CodeWord[0] = gf.add(CodeWord[0], 1); 

    RS_Decoder RS_Dec(n, k, gf); 
    std :: vector <int> ERR_Correc = RS_Dec.decode(CodeWord);
    for(auto x : ERR_Correc)
        std :: cout << x << " "; 
    return 0;
}