#include "Encoder_RS.hpp"
#include "RS_tools.hpp"
#include <algorithm>
#include <iostream>
#include <vector> 
#include <streampu.hpp>
#include <aff3ct.hpp>

using namespace spu;
using namespace spu::module;

Encoder_RS::Encoder_RS(const int n, const int k, const int m, const GaloisField& gf)
    : Stateful(), n(n), k(k), m(m), gf(gf)
{
    const std::string name = "Encoder_RS";
    this->set_name(name);
    this->set_short_name(name);

    auto &tt = this->create_task("process");
    auto p_in  = this->create_socket_in <int>(tt, "in",  k*m);
    auto p_out = this->create_socket_out<int>(tt, "out", n*m);

    this->create_codelet(tt, [p_in, p_out](Module &m, runtime::Task &tt, const size_t frame_id) -> int
    {
        auto &mod = static_cast<Encoder_RS&>(m);
        mod._process(static_cast<int*>(tt[p_in].get_dataptr()),
                     static_cast<int*>(tt[p_out].get_dataptr()),
                     frame_id);
        return runtime::status_t::SUCCESS;
    });

    if (n <= k) throw std::invalid_argument("n must be greater than k");

    r = n - k;
    t = r / 2;

    parity.resize(r);
    messages.reserve(k);
}

Encoder_RS* Encoder_RS::clone() const
{
    auto m = new Encoder_RS(*this);
    m->deep_copy(*this);
    return m;
}

void Encoder_RS::encode(const std::vector<int>& message, std::vector<int>& codeword) {
    std::fill(parity.begin(), parity.end(), 0); 

    int* __restrict par = parity.data();
    const int* __restrict mt = gf.get_mul_table().data();
    const int* __restrict msg = message.data();
    
    for (int i = k - 1; i >= 0; --i) {
        int feedback = GF_ADD(msg[i], par[r-1]);
        for (int j = r - 1; j > 0; --j) {
            par[j] = GF_ADD(par[j-1], mt[feedback * r + j]);
        }
        par[0] = mt[feedback * r];
    }
    
    codeword.resize(n);
    std::memcpy(codeword.data(), par, r * sizeof(int));
    std::memcpy(codeword.data() + r, msg, k * sizeof(int));
}

void Encoder_RS::_process(const int* in, int* out, const int frame_id)
{   

    spu :: tools :: Bit_packer :: pack(in, messages.data(), k*m, 1, false, m); 
    encode(messages, CodeWord);
    spu :: tools :: Bit_packer :: unpack(CodeWord.data(), out, n*m, 1, false, m);
    // Minimal example: copy input to output and print trace
    // std::cout << "MyModule processing frame " << frame_id << std::endl;
    //std::copy(in, in + this->n_elmts, out);
}