#pragma once

#include "Variable.h"

/**
*/
struct CallableWithSelfAndArgs {
    void           write_to_stream( std::ostream &os ) const { os << "CallableWithSelfAndArgs(" << callable << ",self=" << self << ")"; }

    Variable       callable; ///< can be a lambda, a surdeflist, ...
    Variable       self;
    Vec<Variable>  args;
    Vec<Rc_string> names;
};
