#pragma once

#include "CatchedVariable.h"
#include "Ast/Ast_crepr.h"
// #include "ValidScopePtr.h"

/**
*/
struct Block {
    void                 write_to_stream( std::ostream &os ) const { os << "Block(...)"; }

    Vec<Ast_crepr>       code;
    Rc_string            name;
    // ValidScopePtr     valid_scope_ptr;
    Vec<CatchedVariable> catched_variables;
};
