#pragma once

#include "Variable.h"

/**
*/
class CatchedVariable {
public:
    void     write_to_stream( std::ostream &os ) const { os << name; }
    bool     operator==     ( const CatchedVariable &c ) const { return name == c.name && val == c.val; }

    RcString name;
    Variable val;
};
