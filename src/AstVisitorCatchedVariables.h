#pragma once

#include "Ast/AstVisitor.h"
#include <stack>
#include <set>

/**
*/
class AstVisitorCatchedVariables : public AstVisitor {
public:
    AstVisitorCatchedVariables();

    void                      pre_visit          ( RcString cm );

    virtual void              enter_variable     ( RcString name ) override;
    virtual void              enter_block        ( const Vec<RcString> &items ) override;
    virtual void              leave_block        () override;
    virtual void              enter_assign       ( RcString name, RcString cname, PI8 nb_scopes_res, RcString value, PI8 flags ) override;
    virtual void              enter_def          ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, RcString self_constraint, RcString return_type, const Vec<RcString> &wpc ) override;
    virtual void              leave_def          () override;
    virtual void              enter_class        ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance ) override;
    virtual void              leave_class        () override;
    virtual void              enter_enum         ( RcString name, const Vec<RcString> &items ) override;
    virtual void              enter_for          ( RcString name, RcString container, const Vec<RcString> &block ) override;
    virtual void              leave_for          () override;
    virtual void              enter_lambda       ( const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &splats, RcString body ) override;
    virtual void              leave_lambda       () override;
    virtual void              enter_try_catch    ( RcString try_body, RcString arg_name, RcString arg_constraint, RcString catch_body, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_splats );
    virtual void              leave_try_catch    ();

    void                      new_var            ( RcString name );
    void                      new_scope          ();
    void                      rem_scope          ();

    std::set<RcString>        internal;
    std::set<RcString>        catched;
    std::stack<Vec<RcString>> local;
};
