#pragma once
#include <streampu.hpp>

namespace spu {
namespace module {

class error_injector : public Stateful
{
private:
    int k, n, m;
    int n_elmts;
public:
    
    error_injector(int n, int k, int m, const int n_elmts);
    virtual ~error_injector() = default;
    
    virtual error_injector* clone() const override;

protected:
    void _process(const int* in, int* out, const int frame_id);
};

}
}