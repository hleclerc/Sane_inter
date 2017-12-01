#pragma once

#include "System/LVec.h"

/**
*/
struct CtMap {
    void write_to_stream( std::ostream &os ) const { os << keys << "->" << vals; }

    LVec<Variable> keys;
    LVec<Variable> vals;
};
