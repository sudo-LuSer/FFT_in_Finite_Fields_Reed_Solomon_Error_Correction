#include "error_injector.hpp"
#include <algorithm>
#include <iostream>

using namespace spu;
using namespace spu::module;

error_injector::error_injector(int n, int k, int m, const int n_elmts)
: Stateful(), n(n), k(k), m(m),n_elmts(n_elmts)
{
    const std::string name = "error_injector";
    this->set_name(name);
    this->set_short_name(name);

    auto &t = this->create_task("process");
    auto p_in  = this->create_socket_in <int>(t, "in",  n*m);
    auto p_out = this->create_socket_out<int>(t, "out", n*m);

    this->create_codelet(t, [p_in, p_out](Module &m, runtime::Task &t, const size_t frame_id) -> int
    {
        auto &mod = static_cast<error_injector&>(m);
        mod._process(static_cast<int*>(t[p_in].get_dataptr()),
                     static_cast<int*>(t[p_out].get_dataptr()),
                     frame_id);
        return runtime::status_t::SUCCESS;
    });
}

error_injector* error_injector::clone() const
{
    auto m = new error_injector(*this);
    m->deep_copy(*this);
    return m;
}

void error_injector::_process(const int* in, int* out, const int frame_id)
{
    int p[n * m];
    std::copy(in, in + n*m, p); // Copy input to local buffer
    int t = n_elmts; 
    int i = 0;
    while(t--){
        p[i] = !p[i]; // Inject error by flipping bits (for demonstration)
        i ++; 
    }
    // Minimal example: copy input to output and print trace
    // std::cout << "error_injector processing frame " << frame_id << std::endl
    std::copy(p, p + n * m, out); // Copy local buffer to output (for demonstration)
}