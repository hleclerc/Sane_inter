#pragma once

#include "Variable.h"
#include "System/Rc_string.h"

/**
*/
struct Wildcard {
    void write_to_stream( std::ostream &os ) const { os << "Wildcard(" << name << ")"; }

    Rc_string name;
};
