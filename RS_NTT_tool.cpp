#include "RS_NTT_tool.hpp"

using namespace std;

NTT_GF::NTT_GF(GaloisField& field) : gf(field) {}

vector<int> NTT_GF::factorize(int n) {
    vector<int> factors;
    int temp = n;
    
    for (int i = 2; i * i <= temp; i++) {
        if (temp % i == 0)
            factors.push_back(i);
        while (temp % i == 0) {
            temp /= i;
        }
    }

    if (temp > 1) {
        factors.push_back(temp);
    }
    
    return factors;
}

int NTT_GF::find_primitive_root(int n) {
    int field_size = gf.get_size();

    if ((field_size - 1) % n == 0) {
        int exponent = (field_size - 1) / n;
        int generator = gf.get_alpha_to()[1];
        return gf.pow_gf(generator, exponent);
    }

    else{
        throw runtime_error ("NTT impossible: n ne divise pas (2^m - 1). n=" + 
                           to_string(n) + ", 2^m-1=" + to_string(field_size - 1));
        return 0; 
    }
    
    throw runtime_error ("Pas de racine primitive trouvée pour n=" + to_string(n));
}

// Transformation NTT (Cooley-Tukey)
void NTT_GF::ntt_transform(vector<int>& a, int root, bool inverse) {
    int n = a.size();
    
    if ((n & (n - 1)) != 0) {
        throw runtime_error("NTT nécessite une taille puissance de 2. n=" + to_string(n));
    }
    
    // PERMUTATION PAR INVERSION DE BITS 2⁻k = 2^n-k 
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        
        if (i < j) {
            swap(a[i], a[j]);
        }
    }
    
    // (BUTTERFLY) 
    for (int len = 2; len <= n; len <<= 1) {
        // Racine pour cette longueur
        int wlen = root;
        
        // wlen = root^(n/len)
        for (int i = len; i < n; i <<= 1) {
            wlen = gf.mul(wlen, wlen);
        }
        
        // Parcourir les blocs
        for (int i = 0; i < n; i += len) {
            int w = 1;
            
            // Opération papillon dans chaque bloc
            for (int j = 0; j < len/2; j++) {
                int u = a[i + j];
                int v = gf.mul(a[i + j + len/2], w);
                
                // a[i+j] = u + v
                // a[i+j+len/2] = u - v
                a[i + j] = gf.add(u, v);
                a[i + j + len/2] = gf.sub(u, v);
                
                // w = w * wlen
                w = gf.mul(w, wlen);
            }
        }
    }
    
    // === 3. TRANSFORMÉE INVERSE ===
    if (inverse) {
        int n_inv = gf.div(1, n);
        for (int& x : a) {
            x = gf.mul(x, n_inv);
        }
        reverse(a.begin() + 1, a.end());
    }
}

// Multiplication polynomiale via NTT
vector<int> NTT_GF::multiply_polynomials(const vector<int>& a, const vector<int>& b) {
    int result_size = a.size() + b.size() - 1;
    
    int n = 1;
    while (n < result_size) {
        n <<= 1;
    }
    
    // Préparer les vecteurs étendus
    vector<int> fa(n, 0);
    vector<int> fb(n, 0);
    
    copy(a.begin(), a.end(), fa.begin());
    copy(b.begin(), b.end(), fb.begin());
    
    try {
        int root = find_primitive_root(n);
        
        ntt_transform(fa, root, false);
        ntt_transform(fb, root, false);
        
        for (int i = 0; i < n; i++) {
            fa[i] = gf.mul(fa[i], fb[i]);
        }

        ntt_transform(fa, root, true);
        
        fa.resize(result_size);
        
        return fa;
        
    } 
    catch (const exception& e){
        throw runtime_error("Erreur dans multiply_polynomials: " + string(e.what()));
    }
}

vector<int> NTT_GF::convolve(const vector<int>& a, const vector<int>& b) {
    return multiply_polynomials(a, b);
}
