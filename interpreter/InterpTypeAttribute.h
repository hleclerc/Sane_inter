#pragma once

#include "String.h"

/**
*/
struct InterpTypeAttribute {
    void write_to_stream( std::ostream &os ) const { os << name << " off=" << offset; }

    String  name;
    Type   *type;
    SI32    offset;
    SI32    _smurf;
};
