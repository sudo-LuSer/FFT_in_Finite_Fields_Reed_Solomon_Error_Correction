#pragma once
#include <streampu.hpp>
#include "RS_Decoder.hpp"
#include "RS_tools.hpp"
#include "Encoder_RS.hpp"

namespace spu {
namespace module {

class Decoder_RS : public Stateful
{
private:
    int n,k,m;
    GaloisField gf; 
    RS_Decoder RS_Dec;
    std :: vector <int> CodeWord;
public:
    Decoder_RS(const int n, const int k, const int m);
    virtual ~Decoder_RS() = default;
    
    virtual Decoder_RS* clone() const override;

protected:
    void _process(const int* in, int* out, const int frame_id);
};

}
}