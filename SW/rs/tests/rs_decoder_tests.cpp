#include "RS_Decoder.hpp"
#include "RS_tools.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

static void require(bool ok,const std::string& msg) {
    if(!ok) throw std::runtime_error(msg);
}
static std::vector<int> generator(const GaloisField& gf,int r,int first) {
    const int order=gf.get_size()-1;
    const auto& alpha=gf.get_alpha_to();
    std::vector<int> g(1,1);
    for(int j=0;j<r;++j) {
        int e=(first+j)%order; if(e<0)e+=order;
        const int root=alpha[static_cast<std::size_t>(e)];
        std::vector<int> next(g.size()+1,0);
        for(std::size_t i=0;i<g.size();++i) {
            next[i]^=gf.mul(g[i],root);
            next[i+1]^=g[i];
        }
        g.swap(next);
    }
    return g;
}
static std::vector<int> encode(const GaloisField& gf,const std::vector<int>& msg,int n,int first) {
    const int k=static_cast<int>(msg.size()),r=n-k;
    const auto g=generator(gf,r,first);
    std::vector<int> work(static_cast<std::size_t>(n),0);
    for(int i=0;i<k;++i) work[static_cast<std::size_t>(r+i)]=msg[static_cast<std::size_t>(i)];
    for(int deg=n-1;deg>=r;--deg) {
        const int c=work[static_cast<std::size_t>(deg)];
        if(!c)continue;
        for(int j=0;j<=r;++j)
            work[static_cast<std::size_t>(deg-r+j)]^=gf.mul(c,g[static_cast<std::size_t>(j)]);
    }
    std::vector<int> cw(static_cast<std::size_t>(n),0);
    std::copy(work.begin(),work.begin()+r,cw.begin());
    std::copy(msg.begin(),msg.end(),cw.begin()+r);
    return cw;
}
static void test_inverses(int m) {
    GaloisField gf(m);
    for(int a=1;a<gf.get_size();++a)
        require(gf.mul(a,gf.inv(a))==1,"inverse identity failed for m="+std::to_string(m));
}
static void test_rs(int m,int n,int k,int first,int trials,unsigned seed) {
    GaloisField gf(m); RS_Decoder decoder(n,k,gf,first);
    const int t=(n-k)/2;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> symbol(0,gf.get_size()-1);
    std::uniform_int_distribution<int> err(1,gf.get_size()-1);
    std::vector<int> positions(static_cast<std::size_t>(n));
    for(int i=0;i<n;++i)positions[static_cast<std::size_t>(i)]=i;
    for(int trial=0;trial<trials;++trial) {
        std::vector<int> msg(static_cast<std::size_t>(k));
        for(int& x:msg)x=symbol(rng);
        const auto cw=encode(gf,msg,n,first);
        const auto s=decoder.compute_syndromes(cw);
        require(std::all_of(s.begin(),s.end(),[](int x){return x==0;}),"encoder produced nonzero syndrome");
        auto received=cw;
        std::shuffle(positions.begin(),positions.end(),rng);
        const int count=trial%(t+1);
        for(int i=0;i<count;++i)
            received[static_cast<std::size_t>(positions[static_cast<std::size_t>(i)])]^=err(rng);
        std::vector<int> corrected;
        require(decoder.try_decode(received,corrected),
            "decoder rejected correctable word m="+std::to_string(m)+", n="+std::to_string(n)+
            ", r="+std::to_string(n-k)+", first_root="+std::to_string(first)+", errors="+std::to_string(count));
        require(corrected==cw,"decoded codeword differs from original");
    }
}
int main() {
    try {
        test_inverses(3); test_inverses(4); test_inverses(8);
        test_rs(3,7,3,1,80,0x1234u);
        test_rs(3,7,3,0,80,0x2234u);
        test_rs(4,15,10,1,100,0x3344u); // odd redundancy
        test_rs(4,15,10,0,100,0x4455u);
        test_rs(8,255,223,1,100,0x5566u);
        // Exercises mixed-radix FFT syndrome evaluation and Chien search.
        test_rs(12,4095,3935,1,90,0x6677u);
        std::cout<<"All RS decoder tests passed.\n";
        return 0;
    } catch(const std::exception& e) {
        std::cerr<<"TEST FAILURE: "<<e.what()<<'\n';
        return 1;
    }
}
