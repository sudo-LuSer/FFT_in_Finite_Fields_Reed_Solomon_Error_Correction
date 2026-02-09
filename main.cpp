#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include <iostream>
int main(){
    GaloisField gf(3);
    vector <int> messages;
    //messages.push_back(gf.get_alpha_to()[0]);  // 1
    messages.push_back(gf.get_alpha_to()[1]);  // 2
    messages.push_back(gf.get_alpha_to()[2]);  // 4
    messages.push_back(gf.get_alpha_to()[3]);  // 3
    int n = 7, k = 3; 
    RS_Encoder RS_Enc(n,k);
    RS_Enc.build_generator(gf);
    vector<int> gen = RS_Enc.get_generator();
    cout << "Generator polynomial coefficients (from highest degree to constant): ";
    for (int i = gen.size() - 1; i >= 0; i--) {
        cout << gen[i] << " ";
    }
    cout << endl;
    vector<int> mx_poly(n, 0);
    for (int i = 0; i < k; i++) {
        mx_poly[n - k + i] = messages[i];
    }
    
    vector<int> remainder = RS_Enc.poly_div(mx_poly, gen, gf);

    for (int i = remainder.size() - 1; i >= 0; i--) {
        cout << remainder[i] << " ";
    }
    cout << endl; 
    // Encoder
    cout << "Codeword: ";
    for(auto x : RS_Enc.encode(messages, gf))
        cout << x << " ";
    cout << endl;
    return 0;
}