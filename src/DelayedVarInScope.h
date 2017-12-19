#pragma once

#include "Scope.h"

/**
*/
struct DelayedVarInScope {
    ~DelayedVarInScope() { if ( scope ) scope->delayed_vars[ name ].remove_first( this ); }
    void      write_to_stream( std::ostream &os ) const { os << "DelayedVarInScope(" << name << ")"; }

    Scope    *scope;
    RcString  name;
    Variable  value;
};
