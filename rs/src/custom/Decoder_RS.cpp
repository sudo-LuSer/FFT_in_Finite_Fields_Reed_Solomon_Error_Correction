#include "Decoder_RS.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include "RS_Decoder.hpp"
#include "RS_tools.hpp"
#include <streampu.hpp>

using namespace spu;
using namespace spu::module;

Decoder_RS::Decoder_RS(const int n, const int k, const int m)
: Stateful(), n(n), k(k), m(m),gf(m), RS_Dec(n,k,gf){
    const std::string name = "Decoder_RS";
    this->set_name(name);
    this->set_short_name(name);

    auto &t = this->create_task("process");
    auto p_in  = this->create_socket_in <int>(t, "in",  n*m);
    auto p_out = this->create_socket_out<int>(t, "out", k*m);

    this->create_codelet(t, [p_in, p_out](Module &m, runtime::Task &t, const size_t frame_id) -> int
    {
        auto &mod = static_cast<Decoder_RS&>(m);
        mod._process(static_cast<int*>(t[p_in].get_dataptr()),
                     static_cast<int*>(t[p_out].get_dataptr()),
                     frame_id);
        return runtime::status_t::SUCCESS;
    });

    CodeWord.resize(n, 0); 
}

Decoder_RS* Decoder_RS::clone() const
{
    auto m = new Decoder_RS(*this);
    m->deep_copy(*this);
    return m;
}

void Decoder_RS::_process(const int* in, int* out, const int frame_id) {
    spu::tools::Bit_packer::pack(in, CodeWord.data(), n*m, 1, false, m);
    std::vector<int> corrected_codeword = RS_Dec.decode(CodeWord);
    //(message systématique)::
    std::vector<int> message(corrected_codeword.end() - k, corrected_codeword.end());
    spu::tools::Bit_packer::unpack(message.data(), out, k*m, 1, false, m);
}