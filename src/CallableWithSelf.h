#pragma once

#include "Variable.h"

/**
*/
class CallableWithSelf {
public:
    void     write_to_stream( std::ostream &os ) const { os << "CallableWithSelf(" << callable << "," << self << ")"; }

    Variable callable;
    Variable self;
};
