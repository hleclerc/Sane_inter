#pragma once

#include "System/LString.h"
#include "System/LVec.h"

/**
*/
struct TypeContent {
    void write_to_stream( std::ostream &os ) const {
        os << name;
    }

    LString name;
};
