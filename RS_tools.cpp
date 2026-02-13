#include "RS_tools.hpp"

int GaloisField ::  mul(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int log_a = index_of[a];
    int log_b = index_of[b];
    int log_sum = (log_a + log_b) % (size - 1);
    return alpha_to[log_sum];
}
int GaloisField ::  div(int a, int b){
    if (b == 0) throw invalid_argument("Division by zero");
    if (a == 0) return 0;
    int log_a = index_of[a];
    int log_b = index_of[b];
    int log_diff = (log_a - log_b + (size - 1)) % (size - 1);
    return alpha_to[log_diff];
}
int GaloisField ::  add(int a, int b){return a ^ b;} 
int GaloisField ::  sub(int a, int b){return a ^ b;}
int GaloisField ::  pow_gf(int a, int n){
    int log_a = index_of[a]; 
    int log_res = (log_a * (n % (size - 1)) ) % (size - 1); 
    return alpha_to[log_res]; 
}

int GaloisField :: get_size() const { return size; }
int GaloisField :: get_m() const { return m; }
int GaloisField :: get_prim_poly() const {return prim_poly ;}

const vector<int> &GaloisField ::  get_alpha_to() const { return alpha_to; }
const vector<int> &GaloisField ::  get_index_of() const { return index_of; }
