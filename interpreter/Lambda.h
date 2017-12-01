#pragma once

// #include "ValidScopePtr.h"
#include "CatchedVariable.h"
#include "Ast/Ast_crepr.h"
#include "Variable.h"

/**
*/
class Lambda {
public:
    Lambda( const Lambda &l ) {
        arg_names         = l.arg_names;
        arg_constraints   = l.arg_constraints;
        arg_def_vals      = l.arg_def_vals;
        arg_spreads       = l.arg_spreads;
        with_names        = l.with_names;
        with_constraints  = l.with_constraints;
        with_def_vals     = l.with_def_vals;
        with_spreads      = l.with_spreads;
        catched_variables = l.catched_variables;
        body              = l.body;
        source            = l.source;
        offset            = l.offset;
    }
    Lambda() {}

    void                      write_to_stream( std::ostream &os ) const { os << "Lambda(" << arg_names << ")"; }
    bool                      operator==     ( const Lambda &l ) const { return source == l.source && offset == l.offset && catched_variables == l.catched_variables; }


    Vec<Rc_string>            arg_names;
    Vec<Ast_crepr>            arg_constraints;
    Vec<Ast_crepr>            arg_def_vals;
    Vec<PT>                   arg_spreads;

    Vec<Rc_string>            with_names;
    Vec<Ast_crepr>            with_constraints;
    Vec<Ast_crepr>            with_def_vals;
    Vec<PT>                   with_spreads;

    Vec<CatchedVariable>      catched_variables;
    Ast_crepr                 body;

    Rc_string                 source;
    PT                        offset;
};
