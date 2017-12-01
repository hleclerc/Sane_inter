#pragma once

#include "Def.h"

/**
*/
class FunctionSignature {
public:
    FunctionSignature( Def *def );

    void      write_to_stream( std::ostream &os ) const;
    bool      operator<      ( const FunctionSignature &b ) const { return name < b.name; }

    Rc_string name;
};


