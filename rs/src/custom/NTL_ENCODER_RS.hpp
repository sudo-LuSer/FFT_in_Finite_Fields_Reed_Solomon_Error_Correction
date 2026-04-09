#pragma once
#include <streampu.hpp>
#include "RS_ENCODER_NTL.hpp"
#include "RS_tools.hpp"
#include <vector>

// NTL_ENCODER_RS.hpp
namespace spu {
namespace module {

class NTL_ENCODER_RS : public Stateful
{
private:
    int n,k,m;
    std :: vector <int> messages;
    std :: vector <int> CodeWord;
public:
    NTL_ENCODER_RS(const int n, const int k, const int m);
    virtual ~NTL_ENCODER_RS() = default;
    virtual NTL_ENCODER_RS* clone() const override;
protected:
    void _process(const int* in, int* out, const int frame_id);
};

}
}