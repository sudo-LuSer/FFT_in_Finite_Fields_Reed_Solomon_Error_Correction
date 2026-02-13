#include "RS_Encoder.hpp"
#include <iostream>

using namespace std;

void RS_Encoder::build_generator(GaloisField &gf) {
    // CORRECTION : On part proprement du polynôme "1" (degré 0)
    generator.assign(1, 1); 

    int redundancy = n-k;

    for (int i = 1; i <= redundancy; i++) {
        int alpha_i = gf.get_alpha_to()[i % (gf.get_size() - 1)];
        generator = poly_mult_by_binomial(generator, alpha_i, gf);
    }
}

vector<int> RS_Encoder::poly_mult_by_binomial(const vector<int>& poly, int a, GaloisField &gf) {
    int degree = poly.size();
    vector<int> result(degree + 1, 0);
    
    for (int j = 0; j < degree; j++) {
        result[j + 1] = poly[j];
    }
    
    for (int j = 0; j < degree; j++) {
        if (poly[j] != 0) {
            result[j] = gf.add(result[j], gf.mul(poly[j], a));
        }
    }
    return result;
}

vector<int> RS_Encoder::poly_div(const vector<int>& dividend, const vector<int>& divisor, GaloisField &gf) {
    vector<int> remainder = dividend;
    
    // Sécurité anti-boucle infinie : si diviseur vide ou nul
    if (divisor.empty() || divisor.back() == 0) {
        throw runtime_error("Erreur critique: Diviseur invalide (zero ou vide)");
    }

    int deg_d = divisor.size() - 1;
    
    while(remainder.size() >= divisor.size()) {
        int deg_r = remainder.size() - 1;
        
        // Optimisation : Si le coeff dominant est 0, on le vire (ne devrait pas arriver avec la logique ci-dessous, mais sécure)
        if (remainder.back() == 0) {
            remainder.pop_back();
            continue;
        }

        int shift = deg_r - deg_d;
        int coef = gf.div(remainder[deg_r], divisor[deg_d]);
        
        for(int i = 0; i <= deg_d; i++) {
            if (i + shift < (int)remainder.size()){
                remainder[i + shift] = gf.sub(remainder[i + shift], gf.mul(coef, divisor[i]));
            }
        }
        
        // Nettoyage des zéros de tête impératif pour réduire la taille
        while(!remainder.empty() && remainder.back() == 0) {
            remainder.pop_back();
        }
    }
    
    return remainder;
}

vector<int> RS_Encoder::encode(const vector<int>& message, GaloisField &gf) {
    if (message.size() != (size_t)k) {
        throw invalid_argument("Message length must be k = " + to_string(k));
    }
    
    // Si le générateur est vide (grâce au fix du .hpp), on le construit
    if (generator.empty()) {
        build_generator(gf);
    }
 
    vector<int> mx_poly(n, 0);
    for (int i = 0; i < k; i++) {
        mx_poly[n - k + i] = message[i];
    }
    
    vector<int> remainder = poly_div(mx_poly, generator, gf);
        
    // CORRECTION : Le reste doit avoir une taille fixe de n-k pour la concaténation
    // Si poly_div rend un vecteur plus petit (ex: [0]), il faut rajouter des zéros
    remainder.resize(n - k, 0);
    
    vector<int> codeword = remainder;
    codeword.insert(codeword.end(), message.begin(), message.end());
    
    return codeword;
}