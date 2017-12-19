#pragma once

#include "System/RcString.h"
#include "Variable.h"

/**
*/
struct Wildcard {
    void     write_to_stream( std::ostream &os ) const { os << "Wildcard(" << name << ")"; }

    RcString name;
};
