#ifndef NTT_HPP
#define NTT_HPP

#include "RS_tools.hpp"

class NTT_GF {
    private:

        GaloisField& gf;
        int find_primitive_root(int n);
        vector<int> factorize(int n);
        
    public:

        NTT_GF(GaloisField& field);

        void ntt_transform(std::vector<int>& a, int root, bool inverse);
        std::vector<int> multiply_polynomials(const std::vector<int>& a, const std::vector<int>& b);
        std::vector<int> convolve(const vector<int>& a, const vector<int>& b);
        
        GaloisField& get_gf() const {return gf;}
};

#endif 