#pragma once
#include <streampu.hpp>
#include "RS_Encoder.hpp"
#include "RS_tools.hpp"
#include "RS_Decoder.hpp"
#include <vector>


namespace spu {
namespace module {

class Encoder_RS : public Stateful
{
private:
    int n,k,m;
    RS_Encoder RS_Enc;
    GaloisField gf;
    std :: vector <int> messages;
    std :: vector <int> CodeWord;
public:
    Encoder_RS(const int n, const int k, const int m);
    virtual ~Encoder_RS() = default;
    virtual Encoder_RS* clone() const override;
protected:
    void _process(const int* in, int* out, const int frame_id);
};

}
}