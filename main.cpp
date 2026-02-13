#include <iostream>
#include <vector>
#include <iomanip>
#include "RS_tools.hpp"
#include "RS_Encoder.hpp"

using namespace std;

// Fonction utilitaire pour évaluer un polynôme en un point x (Méthode de Horner)
int poly_eval(const vector<int>& poly, int x, GaloisField& gf) {
    int result = 0;
    // Attention : dans ton code, poly[0] est le coefficient de plus bas degré ? 
    // D'après ta concaténation : [Parité | Message], le message est à la fin.
    // Dans RS systématique standard, souvent :
    // C(x) = p_0 + p_1 x + ... + m_0 x^{n-k} + ...
    // Ton implémentation met le message à la fin du vecteur.
    // Supposons : index 0 = coeff x^0 (poids faible).
    
    for (int i = poly.size() - 1; i >= 0; i--) {
        result = gf.add(gf.mul(result, x), poly[i]);
    }
    return result;
}

// Fonction pour afficher un polynôme joliment (puissances de alpha)
void print_poly(const vector<int>& poly, GaloisField& gf) {
    cout << "[ ";
    for (size_t i = 0; i < poly.size(); i++) {
        if (poly[i] == 0) {
            cout << "0 ";
        } else {
            int power = gf.get_index_of()[poly[i]];
            if (power == 0) cout << "1 ";
            else cout << "a^" << power << " ";
        }
    }
    cout << "]" << endl;
}

int main() {
    try {
        cout << "=== TEST REED-SOLOMON GF(2^3) ===" << endl;

        // 1. Initialisation du corps GF(8) -> polynôme primitif x^3 + x + 1 (11)
        GaloisField gf(3);
        
        // 2. Paramètres du code : RS(7, 3)
        // n = 7 (longueur totale)
        // k = 3 (longueur message)
        // 2t = n - k = 4 symboles de parité (capacité de correction t=2)
        int n = 7;
        int k = 3;
        RS_Encoder encoder(n, k);

        cout << "Code: RS(" << n << ", " << k << ") sur GF(" << gf.get_size() << ")" << endl;
        cout << "Capacite de correction t = " << (n-k)/2 << " erreurs." << endl;
        cout << "-----------------------------------" << endl;

        // 3. Création du message
        // Exemple : Message = (alpha^1, alpha^2, alpha^3)
        vector<int> message;
        //message.push_back(1); // alpha^0
        message.push_back(gf.get_alpha_to()[1]); // alpha^1
        message.push_back(gf.get_alpha_to()[2]); // alpha^2
        message.push_back(gf.get_alpha_to()[3]); // alpha^3

        cout << "Message (k=" << k << ") : ";
        print_poly(message, gf);

        // 4. Encodage
        vector<int> codeword = encoder.encode(message, gf);

        cout << "Mot de code (n=" << n << ") : ";
        print_poly(codeword, gf);
        
        // Affichage en entiers bruts pour débogage
        cout << "Valeurs brutes : ";
        for(int x : codeword) cout << x << " ";
        cout << endl;

        cout << "-----------------------------------" << endl;
        cout << "VERIFICATION DE VALIDITE (Syndrome Check)" << endl;
        
        // 5. Test de validité mathématique
        // Le mot de code doit s'évaluer à 0 pour les racines du générateur.
        // Racines habituelles : alpha^1, alpha^2, ..., alpha^{2t}
        // Ici 2t = 4, donc racines : alpha^1, alpha^2, alpha^3, alpha^4
        
        bool valid = true;
        int num_parity = n - k; // 4

        for (int i = 1; i <= num_parity; i++) {
            // On récupère la racine alpha^i
            int root = gf.get_alpha_to()[i];
            
            // On évalue le mot de code en cette racine
            int syndrome = poly_eval(codeword, root, gf);
            
            cout << "Evaluation en alpha^" << i << " : " 
                 << (syndrome == 0 ? "OK (0)" : "ERREUR (" + to_string(syndrome) + ")") << endl;
            
            if (syndrome != 0) valid = false;
        }

        cout << "-----------------------------------" << endl;
        if (valid) {
            cout << ">>> SUCCES : L'encodage est mathématiquement valide !" << endl;
        } else {
            cout << ">>> ECHEC : Le mot de code n'est pas valide." << endl;
        }

    } catch (const exception& e) {
        cerr << "Erreur critique : " << e.what() << endl;
    }

    return 0;
}