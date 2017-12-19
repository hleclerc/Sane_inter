#pragma once

#include "System/LString.h"
#include "System/LVec.h"
#include "Variable.h"

/**
*/
struct Varargs {
    void           write_to_stream( std::ostream &os ) const { os << values << " names=" << names; }

    LVec<Variable> values;
    LVec<LString>  names;
};
