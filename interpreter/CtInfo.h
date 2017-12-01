#pragma once

#include "Variable.h"

/***/
class CtInfo {
public:
    CtInfo( Scope *scope, const Variable &var );

    void write_to_stream( std::ostream &os ) const;

    Scope         *scope;
    const Variable &var;
};
