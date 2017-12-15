#pragma once

#include "Value.h"

/**
*/
class Ref : public RcObj {
public:
    virtual ~Ref();

    virtual void write_to_stream( std::ostream &os ) const = 0;
};

