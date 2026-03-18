#include "Encoder_RS.hpp"
#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include "RS_Decoder.hpp"
#include <algorithm>
#include <iostream>
#include <vector> 
#include <streampu.hpp>
#include <aff3ct.hpp>

using namespace spu;
using namespace spu::module;

Encoder_RS::Encoder_RS(const int n, const int k, const int m)
    : Stateful(), n(n), k(k), m(m), gf(m), RS_Enc(n,k, gf)
{
    const std::string name = "Encoder_RS";
    this->set_name(name);
    this->set_short_name(name);

    auto &t = this->create_task("process");
    auto p_in  = this->create_socket_in <int>(t, "in",  k*m);
    auto p_out = this->create_socket_out<int>(t, "out", n*m);

    this->create_codelet(t, [p_in, p_out](Module &m, runtime::Task &t, const size_t frame_id) -> int
    {
        auto &mod = static_cast<Encoder_RS&>(m);
        mod._process(static_cast<int*>(t[p_in].get_dataptr()),
                     static_cast<int*>(t[p_out].get_dataptr()),
                     frame_id);
        return runtime::status_t::SUCCESS;
    });
    messages.resize(k,0); 
    CodeWord.resize(n,0); 
}

Encoder_RS* Encoder_RS::clone() const
{
    auto m = new Encoder_RS(*this);
    m->deep_copy(*this);
    return m;
}

void Encoder_RS::_process(const int* in, int* out, const int frame_id)
{   

    spu :: tools :: Bit_packer :: pack(in, messages.data(), k*m, 1, false, m); 
    RS_Enc.encode(messages, CodeWord);
    spu :: tools :: Bit_packer :: unpack(CodeWord.data(), out, n*m, 1, false, m);
    // Minimal example: copy input to output and print trace
    // std::cout << "MyModule processing frame " << frame_id << std::endl;
    //std::copy(in, in + this->n_elmts, out);
}