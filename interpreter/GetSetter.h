#pragma once

#include "Variable.h"

/**
*/
struct GetSetter {
    GetSetter() : type( 0 ) {}

    void     write_to_stream( std::ostream &os ) const;
    Type    *get_type       ( Scope *scope, Variable *get_res = 0 );
    Variable get_var        ( Scope *scope, bool rec = true );

    Variable get;
    Variable set;
    Variable mod;
    Variable type_of;

    Variable var;  ///< buffer for result of get. cleared after a set or a mod. May be a GetSetter
    Type    *type; ///< buffer for the type of the variable. Never cleared (we assume constant type)
};
