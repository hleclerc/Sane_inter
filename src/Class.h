#pragma once

#include "CatchedVariable.h"
#include "System/Deque.h"
#include "Ast/AstCrepr.h"
#include "Variable.h"

/**
*/
class Class {
public:
    void                      write_to_stream         ( std::ostream &os ) const { os << "Class(" << name << ")"; }

    RcString                  name; //
    Vec<CatchedVariable>      catched_variables_prep;

    Vec<RcString>             arg_names;
    Vec<AstCrepr>             arg_constraints;
    Vec<AstCrepr>             arg_def_vals;
    Vec<PT>                   arg_spreads;

    Vec<RcString>             with_names;
    Vec<AstCrepr>             with_constraints;
    Vec<AstCrepr>             with_def_vals;
    Vec<PT>                   with_spreads;

    AstCrepr                 condition;

    SI32                      pert_code;
    SI32                      def_pert_num;
    SI32                      def_pert_exp;

    Vec<CatchedVariable>      catched_variables;
    // ValidScopePtr          valid_scope_ptr;
    Vec<AstCrepr>             block;

    RcString                  source;
    PT                        offset;

    Vec<AstCrepr>             inheritance;
    Vec<RcString>             inheritance_names;
    Vec<Vec<CatchedVariable>> catched_variables_inh;

    Deque<Type *>             instances;

    Bool                      export_flag;
    Bool                      global_flag;
};
