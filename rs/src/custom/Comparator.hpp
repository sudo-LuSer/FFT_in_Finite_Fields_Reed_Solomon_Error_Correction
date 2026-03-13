#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "streampu.hpp"

namespace spu
{
namespace module
{

class Comparator  : public Stateful {

private:
    
    int frame_size;

public:
    int nb_err = 0; 
    Comparator(int frame_size);
    virtual ~Comparator();

protected:

    virtual void compare(int* input1, int* input2, int *output, const int frame_id);

};
}
}

#endif // COMPARATOR_H