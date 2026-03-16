#include <random>
#include "Comparator.hpp"

using namespace spu;
using namespace spu::module;

    Comparator::Comparator(int frame_size) : Stateful(), frame_size(frame_size) {

        this->set_name("Comparator");
        this->set_short_name("Comparator");

        auto &t = create_task("compare");
        auto input1   = create_socket_in<int>(t, "input1", frame_size);
        auto input2   = create_socket_in<int>(t, "input2", frame_size);
        auto output   = create_socket_out<int>(t, "output", frame_size);

        this->create_codelet(t, [input1, input2, output](Module &m, runtime::Task &t, const size_t frame_id) -> int {
        static_cast<Comparator&>(m).compare(   static_cast<int*>(t[input1].get_dataptr()),
                                                static_cast<int*>(t[input2].get_dataptr()),
                                                static_cast<int*>(t[output].get_dataptr()),
                                                        frame_id);
        return 0;
    });

    }

    Comparator::~Comparator() {
    }    

    void Comparator::compare(int *input1, int *input2, int *output, const int frame_id) {
        
        for(auto i = 0; i < frame_size; i++)
        {
            if(input1[i] == input2[i]) {output[i] = 0;}
            else output[i] = 1;            
        }        
    }