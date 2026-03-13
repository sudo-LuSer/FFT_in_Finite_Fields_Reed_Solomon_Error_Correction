
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <streampu.hpp>

#include <aff3ct.hpp>

#include "custom/Comparator.hpp"

#include "custom/Encoder_RS.hpp"
#include "custom/Decoder_RS.hpp"

using namespace spu;
using namespace spu::module;

int main(int argc, char** argv)
{
    std::cout << "Starting Hulotte project..." << std::endl;


    // 1. Modules creation
    
    // RS(7, 5) => t=1, m=3. Bits 21 -> 15.
    // NOTE: This uses aff3ct B_32 (32-bit integer) template instantiation.
    const int N_rs = 255;  // Symbols
    const int K_rs = 239;  // Symbols
    const int m = 8;       // Bits per symbol
    const int t = (N_rs - K_rs) / 2; // Correction 
    const int N = N_rs * m; // Total bits
    const int K = K_rs * m; // Info bits

    module::Source_random<> source(K);
    module::Finalizer    <> finalizer(N);
    module::Finalizer    <> finalizer_(K);

    module::Comparator cmp(N); 
    module::Comparator cmp_(K); 
    
    // Create RS Polynomial Generator (needed for RS construction)
    aff3ct::tools::RS_polynomial_generator poly(N_rs, t);
    
    // Create Encoder and Decoder
    aff3ct::module::Encoder_RS<>     encoder(K_rs, N_rs, poly);
    aff3ct::module::Decoder_RS_std<> decoder(K_rs, N_rs, poly);    

    
    module::Encoder_RS         encoder_rs(N_rs, K_rs, m);
    module::Decoder_RS         decoder_rs(N_rs, K_rs, m);

    // 2. Sockets binding
    using namespace aff3ct::module;
    using namespace aff3ct::tools;
    
    // Chain construction
    
    // Chain: Source -> (Custom Module) -> Encoder
    source   [src::tsk::generate][(int)src::sck::generate::out_data] =  encoder_rs ["process::in"];
    encoder_rs ["process::out"] = cmp["compare :: input1"]; //finalizer["finalize::in"]; //  encoder  [enc::tsk::encode][(int)enc::sck::encode::U_K];
    source   [src::tsk::generate][(int)src::sck::generate::out_data] = encoder  [enc::tsk::encode][(int)enc::sck::encode::U_K];
    encoder  [enc::tsk::encode][(int)enc::sck::encode::X_N] = cmp["compare::input2"];  
    cmp["compare :: output"] = finalizer["finalize::in"];

    // encoder  [enc::tsk::encode][(int)enc::sck::encode::X_N] = decoder  [dec::tsk::decode_hiho][(int)dec::sck::decode_hiho::Y_N];
    // decoder  [dec::tsk::decode_hiho][(int)dec::sck::decode_hiho::V_K] = cmp_["compare :: input1"]; 
    // encoder  [enc::tsk::encode][(int)enc::sck::encode::X_N] = decoder_rs ["process::in"];
    // decoder_rs["process :: out"] = cmp_["compare :: input2"];
    // cmp_["compare::output"] = finalizer_["finalize::in"];

    // 3. Sequence creation
    std::vector<runtime::Task*> first_tasks;

    first_tasks.push_back(&source("generate"));


    runtime::Sequence sequence(first_tasks);

    // Configuration
    for (auto& type : sequence.get_tasks_per_types())
        for (auto& t : type)
        {
            t->set_stats(true);
            t->set_debug(true);
        }

    // 4. Execution
    std::cout << "Processing..." << std::endl;
    
    // Export dot file for visualization
    std::ofstream file("graph.dot");
    sequence.export_dot(file);

    // Run the sequence
    for (auto i = 0; i < 1; i++)
        sequence.exec_seq(); // Run 1 frame at a time

    // 5. Stats
    std::cout << "\\nEnd of execution." << std::endl;
    tools::Stats::show(sequence.get_tasks_per_types(), true, false);

    return 0;
}
