#include <iostream>
#include <vector>
#include "RS_tools.hpp"
#include "RS_Encoder.hpp"
#include "RS_Decoder.hpp"

int main() {
    const int m=3, n=(1<<m)-1, k=3;
    GaloisField gf(m);
    gf.define_generator(n-k);

    RS_Encoder encoder(n,k,gf);
    RS_Decoder decoder(n,k,gf);

    const std::vector<int> message{1,2,3};
    std::vector<int> codeword;
    encoder.encode(message,codeword);

    std::vector<int> received=codeword;
    received[1]^=3; // two symbol errors, within t=2
    received[5]^=6;

    std::cout<<"Original codeword : ";
    for(int x:codeword) std::cout<<x<<' ';
    std::cout<<"\nReceived word    : ";
    for(int x:received) std::cout<<x<<' ';

    const std::vector<int> corrected=decoder.decode(received);
    std::cout<<"\nCorrected word   : ";
    for(int x:corrected) std::cout<<x<<' ';
    const std::vector<int> decoded_message(corrected.end()-k,corrected.end());
    std::cout<<"\nDecoded message  : ";
    for(int x:decoded_message) std::cout<<x<<' ';
    std::cout<<'\n';
    return decoded_message==message ? 0 : 1;
}
