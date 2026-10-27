#include "Decoder_RS.hpp"
#include <streampu.hpp>
using namespace spu;
using namespace spu::module;

Decoder_RS::Decoder_RS(const int n, const int k, const int m, const int first_root)
: Stateful(), n(n), k(k), m(m), gf(m), RS_Dec(n,k,gf,first_root) {
    const std::string name = "Decoder_RS";
    this->set_name(name);
    this->set_short_name(name);
    auto &task = this->create_task("process");
    auto p_in = this->create_socket_in<int>(task, "in", n*m);
    auto p_out = this->create_socket_out<int>(task, "out", k*m);
    this->create_codelet(task, [p_in,p_out](Module &module, runtime::Task &task, const size_t frame_id) -> int {
        auto &decoder = static_cast<Decoder_RS&>(module);
        decoder._process(static_cast<int*>(task[p_in].get_dataptr()),
                         static_cast<int*>(task[p_out].get_dataptr()), frame_id);
        return runtime::status_t::SUCCESS;
    });
    CodeWord.resize(n, 0);
}
Decoder_RS* Decoder_RS::clone() const {
    auto *copy = new Decoder_RS(*this);
    copy->deep_copy(*this);
    return copy;
}
void Decoder_RS::_process(const int* in, int* out, const int frame_id) {
    spu::tools::Bit_packer::pack(in, CodeWord.data(), n*m, 1, false, m);
    const std::vector<int> corrected = RS_Dec.decode(CodeWord);
    // This library stores systematic words as [parity | message].
    const std::vector<int> message(corrected.end()-k, corrected.end());
    spu::tools::Bit_packer::unpack(message.data(), out, k*m, 1, false, m);
}
