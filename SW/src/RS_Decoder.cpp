#include "RS_Decoder.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>

namespace {
int smallest_factor(int x) {
    if (x % 2 == 0) return 2;
    for (int p = 3; static_cast<std::int64_t>(p)*p <= x; p += 2)
        if (x % p == 0) return p;
    return x;
}
bool factor_fft_size(int length, int max_radix, int& cost) {
    cost = 0;
    while (length > 1) {
        const int radix = smallest_factor(length);
        if (radix > max_radix) return false;
        cost += radix + 1;
        length /= radix;
    }
    return true;
}
int alpha_power(const GaloisField& gf, std::int64_t exponent) {
    const int order = gf.get_size()-1;
    exponent %= order;
    if (exponent < 0) exponent += order;
    return gf.get_alpha_to()[static_cast<std::size_t>(exponent)];
}

// Mixed-radix DFT over GF(2^m): output[j] = sum_i input[i]*alpha^(i*j).
// It is valid only for transform lengths dividing |GF*|; here it is used for
// full-length RS codes n=2^m-1. No radix-2 transform is assumed.
std::vector<int> fft_rec(const std::vector<int>& in, const GaloisField& gf, int root_exp) {
    const int n = static_cast<int>(in.size());
    if (n <= 1) return in;
    const int order = gf.get_size()-1;
    const int radix = smallest_factor(n);
    const int sub_n = n/radix;
    const int sub_root = static_cast<int>((static_cast<std::int64_t>(root_exp)*radix)%order);
    std::vector<std::vector<int>> part(static_cast<std::size_t>(radix),
                                       std::vector<int>(static_cast<std::size_t>(sub_n)));
    std::vector<int> sequence(static_cast<std::size_t>(sub_n));
    for (int r=0; r<radix; ++r) {
        for (int j=0; j<sub_n; ++j)
            sequence[static_cast<std::size_t>(j)] = in[static_cast<std::size_t>(r+radix*j)];
        part[static_cast<std::size_t>(r)] = fft_rec(sequence,gf,sub_root);
    }
    std::vector<int> out(static_cast<std::size_t>(n),0);
    std::vector<int> twiddled(static_cast<std::size_t>(radix),0);
    for (int k0=0; k0<sub_n; ++k0) {
        for (int r=0; r<radix; ++r)
            twiddled[static_cast<std::size_t>(r)] =
                gf.mul(part[static_cast<std::size_t>(r)][static_cast<std::size_t>(k0)],
                       alpha_power(gf,static_cast<std::int64_t>(root_exp)*r*k0));
        for (int k1=0; k1<radix; ++k1) {
            int sum=0;
            for (int r=0; r<radix; ++r)
                sum ^= gf.mul(twiddled[static_cast<std::size_t>(r)],
                    alpha_power(gf,static_cast<std::int64_t>(root_exp)*sub_n*r*k1));
            out[static_cast<std::size_t>(k0+sub_n*k1)] = sum;
        }
    }
    return out;
}
std::vector<int> finite_field_fft(const std::vector<int>& coefficients, const GaloisField& gf) {
    return fft_rec(coefficients,gf,1);
}
bool all_zero(const std::vector<int>& a) {
    return std::all_of(a.begin(),a.end(),[](int x){return x==0;});
}
}

RS_Decoder::RS_Decoder(int n, int k, GaloisField& gf, int first_root)
    : n_(n), k_(k), redundancy_(n-k), t_((n-k)/2),
      first_root_(first_root), gf_(gf) {
    const int order=gf_.get_size()-1;
    if (k_<1 || n_<=k_ || n_>order)
        throw std::invalid_argument("RS parameters must satisfy 1 <= k < n <= 2^m-1");
    int first=first_root_%order;
    if (first<0) first+=order;
    syndrome_roots_.resize(static_cast<std::size_t>(redundancy_));
    const auto& alpha=gf_.get_alpha_to();
    for (int j=0;j<redundancy_;++j)
        syndrome_roots_[static_cast<std::size_t>(j)]=alpha[static_cast<std::size_t>((first+j)%order)];
    inverse_powers_.resize(static_cast<std::size_t>(n_));
    for (int p=0;p<n_;++p)
        inverse_powers_[static_cast<std::size_t>(p)]=alpha[static_cast<std::size_t>((order-(p%order))%order)];

    fft_supported_=(n_==order) && factor_fft_size(n_,41,fft_cost_units_);
}

int RS_Decoder::poly_eval(const std::vector<int>& poly, int x) {
    int y=0;
    for (int i=static_cast<int>(poly.size())-1;i>=0;--i)
        y=gf_.mul(y,x)^poly[static_cast<std::size_t>(i)];
    return y;
}

std::vector<int> RS_Decoder::compute_syndromes(const std::vector<int>& received) {
    if (received.size()!=static_cast<std::size_t>(n_))
        throw std::invalid_argument("received word length must equal n");
    const int q=gf_.get_size();
    for (int a:received) if (a<0 || a>=q)
        throw std::invalid_argument("received symbol is not in GF(2^m)");

    std::vector<int> s(static_cast<std::size_t>(redundancy_),0);
    // Full multiplicative DFT can evaluate all n roots faster for large n/r.
    if (fft_supported_ && redundancy_>=fft_cost_units_ && n_>=1024) {
        const auto values=finite_field_fft(received,gf_);
        int first=first_root_%(gf_.get_size()-1);
        if (first<0) first+=gf_.get_size()-1;
        for (int j=0;j<redundancy_;++j)
            s[static_cast<std::size_t>(j)]=values[static_cast<std::size_t>((first+j)%n_)];
        return s;
    }
    for (int j=0;j<redundancy_;++j)
        s[static_cast<std::size_t>(j)]=poly_eval(received,syndrome_roots_[static_cast<std::size_t>(j)]);
    return s;
}

std::vector<int> RS_Decoder::berlekamp_massey(const std::vector<int>& s) {
    const int count=static_cast<int>(s.size());
    std::vector<int> C(static_cast<std::size_t>(count+1),0),B(static_cast<std::size_t>(count+1),0);
    std::vector<int> previous(static_cast<std::size_t>(count+1),0);
    C[0]=B[0]=1;
    int degree=0, shift=1, old_discrepancy=1;
    for (int step=0;step<count;++step) {
        int discrepancy=s[static_cast<std::size_t>(step)];
        for (int i=1;i<=degree;++i)
            if (C[static_cast<std::size_t>(i)] && s[static_cast<std::size_t>(step-i)])
                discrepancy^=gf_.mul(C[static_cast<std::size_t>(i)],s[static_cast<std::size_t>(step-i)]);
        if (discrepancy==0) { ++shift; continue; }
        previous=C;
        const int scale=gf_.div(discrepancy,old_discrepancy);
        for (int i=0;i+shift<=count;++i)
            if (B[static_cast<std::size_t>(i)])
                C[static_cast<std::size_t>(i+shift)]^=gf_.mul(scale,B[static_cast<std::size_t>(i)]);
        if (2*degree<=step) {
            degree=step+1-degree;
            B.swap(previous); // B becomes the old C, not the updated C.
            old_discrepancy=discrepancy;
            shift=1;
        } else ++shift;
    }
    C.resize(static_cast<std::size_t>(degree+1));
    return C;
}

std::vector<int> RS_Decoder::chien_search(const std::vector<int>& lambda,
                                           std::vector<int>& error_positions) {
    if (lambda.empty()) throw std::invalid_argument("locator polynomial cannot be empty");
    error_positions.clear();
    const int degree=static_cast<int>(lambda.size())-1;
    std::vector<int> X;
    X.reserve(static_cast<std::size_t>(std::min(t_,degree)));
    if (fft_supported_ && degree>=fft_cost_units_ && n_>=1024) {
        std::vector<int> coeff(static_cast<std::size_t>(n_),0);
        std::copy(lambda.begin(),lambda.end(),coeff.begin());
        const auto values=finite_field_fft(coeff,gf_);
        for (int p=0;p<n_;++p) {
            if (values[static_cast<std::size_t>((n_-p)%n_)]==0) {
                error_positions.push_back(p);
                X.push_back(inverse_powers_[static_cast<std::size_t>(p)]);
            }
        }
        return X;
    }
    for (int p=0;p<n_;++p) {
        const int x=inverse_powers_[static_cast<std::size_t>(p)];
        if (poly_eval(lambda,x)==0) { error_positions.push_back(p); X.push_back(x); }
    }
    return X;
}

std::vector<int> RS_Decoder::forney(const std::vector<int>& lambda,
                                     const std::vector<int>& omega,
                                     const std::vector<int>& error_positions,
                                     const std::vector<int>& X) {
    if (error_positions.size()!=X.size())
        throw std::invalid_argument("error positions and locator roots must have equal size");
    std::vector<int> derivative(lambda.size()>1?lambda.size()-1:0,0);
    for (std::size_t i=1;i<lambda.size();i+=2) derivative[i-1]=lambda[i];
    const int order=gf_.get_size()-1;
    const auto& alpha=gf_.get_alpha_to();
    std::vector<int> values(X.size(),0);
    for (std::size_t i=0;i<X.size();++i) {
        const int num=poly_eval(omega,X[i]);
        const int den=poly_eval(derivative,X[i]);
        if (den==0) throw std::runtime_error("zero Forney denominator");
        int magnitude=gf_.mul(num,gf_.inv(den));
        // For a first root f, the scale factor is locator_root^(f-1).
        if (first_root_!=1) {
            std::int64_t exponent=static_cast<std::int64_t>(first_root_-1)*
                gf_.get_index_of()[static_cast<std::size_t>(X[i])];
            exponent%=order;
            if (exponent<0) exponent+=order;
            magnitude=gf_.mul(magnitude,alpha[static_cast<std::size_t>(exponent)]);
        }
        values[i]=magnitude;
    }
    return values;
}

std::vector<int> RS_Decoder::poly_mult(const std::vector<int>& a,const std::vector<int>& b) {
    if (a.empty()||b.empty()) return {};
    std::vector<int> c(a.size()+b.size()-1,0);
    for (std::size_t i=0;i<a.size();++i) if (a[i])
        for (std::size_t j=0;j<b.size();++j) if (b[j])
            c[i+j]^=gf_.mul(a[i],b[j]);
    return c;
}

bool RS_Decoder::try_decode(const std::vector<int>& received,std::vector<int>& corrected) {
    const auto s=compute_syndromes(received);
    if (all_zero(s)) { corrected=received; return true; }
    const auto lambda=berlekamp_massey(s);
    if (lambda.empty()) return false;
    const int degree=static_cast<int>(lambda.size())-1;
    if (degree<1||degree>t_) return false;
    std::vector<int> positions;
    const auto X=chien_search(lambda,positions);
    if (static_cast<int>(positions.size())!=degree) return false;
    auto omega=poly_mult(s,lambda);
    omega.resize(static_cast<std::size_t>(redundancy_),0);
    std::vector<int> magnitudes;
    try { magnitudes=forney(lambda,omega,positions,X); }
    catch (const std::runtime_error&) { return false; }
    if (std::any_of(magnitudes.begin(),magnitudes.end(),[](int x){return x==0;})) return false;
    std::vector<int> candidate=received;
    for (std::size_t i=0;i<positions.size();++i)
        candidate[static_cast<std::size_t>(positions[i])]^=magnitudes[i];
    if (!all_zero(compute_syndromes(candidate))) return false;
    corrected=std::move(candidate);
    return true;
}

std::vector<int> RS_Decoder::decode(const std::vector<int>& received) {
    std::vector<int> corrected;
    if (!try_decode(received,corrected))
        throw std::runtime_error("Reed-Solomon decoding failed: too many errors or invalid codeword");
    return corrected;
}
