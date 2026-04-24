#pragma once
#include <streampu.hpp>
#include "RS_tools.hpp"
#include <vector>


namespace spu {
namespace module {

class Encoder_RS : public Stateful
{
private:
    int n,k,m,t,r;
    GaloisField gf;
    std :: vector <int> messages;
    std::vector<int> parity;
    std :: vector <int> CodeWord;
public:
    Encoder_RS(const int n, const int k, const int m, const GaloisField& gf);
    virtual ~Encoder_RS() = default;
    virtual Encoder_RS* clone() const override;
    virtual void encode(const std::vector<int>& message,
                std::vector<int>& codeword);
protected:
    void _process(const int* in, int* out, const int frame_id);
};

}
}