#pragma once

// #include "ValidScopePtr.h"
#include "CatchedVariable.h"
#include "Ast/Ast_crepr.h"
#include "Variable.h"

/**
*/
class Def {
public:
    void                      write_to_stream         ( std::ostream &os ) const { os << "Def(" << name << ")"; }

    Rc_string                 name; //
    Vec<CatchedVariable>      catched_variables_prep;

    Vec<Rc_string>            arg_names;
    Vec<Ast_crepr>            arg_constraints;
    Vec<Ast_crepr>            arg_def_vals;
    Vec<PT>                   arg_spreads;

    Vec<Rc_string>            with_names;
    Vec<Ast_crepr>            with_constraints;
    Vec<Ast_crepr>            with_def_vals;
    Vec<PT>                   with_spreads;

    Ast_crepr                 condition;

    SI32                      pert_code;
    SI32                      def_pert_num;
    SI32                      def_pert_exp;

    Vec<CatchedVariable>      catched_variables;
    // ValidScopePtr          valid_scope_ptr;
    Vec<Ast_crepr>            block;

    Rc_string                 source;
    PT                        offset;

    Vec<Ast_crepr>            inheritance;
    Vec<Rc_string>            inheritance_names;
    Vec<Vec<CatchedVariable>> catched_variables_inh;

    Vec<Type *>               instances;

    Ast_crepr                 self_constraint;
    Ast_crepr                 return_type;
    Vec<Ast_crepr>            wpc_code;

    Bool                      export_flag;
    Bool                      global_flag;
    Bool                      static_flag;
    Bool                      self_as_arg_flag;
    Bool                      override_flag;
    Bool                      abstract_flag;
    Bool                      inline_flag;
    Bool                      virtual_flag;
    Bool                      named_converter_flag;
};
