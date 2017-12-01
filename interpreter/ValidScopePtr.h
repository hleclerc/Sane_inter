#pragma once

#include "Scope.h"

/**
*/
class ValidScopePtr {
public:
    ValidScopePtr() : scope( 0 ) {
    }

    ~ValidScopePtr() {
        if ( scope )
            scope->valid_scope_ptrs.erase( this );
    }

    void set_scope( Scope *n_scope ) {
        ASSERT( scope == 0, "" );
        n_scope->valid_scope_ptrs.insert( this );
        scope = n_scope;
    }

    Scope *get_scope() const {
        return scope;
    }


private:
    friend class Scope;

    Scope *scope;
};
