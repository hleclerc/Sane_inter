#pragma once

#include "System/LVec.h"
#include "Variable.h"
#include "String.h"

/**
*/
struct Varargs {
    void write_to_stream( std::ostream &os ) const { os << values << " names=" << names; }

    LVec<Variable>  values;
    LVec<String>    names;
};
