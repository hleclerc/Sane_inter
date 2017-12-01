#pragma once

#include "Ast/Ast_visitor.h"
#include <set>

/**
*/
class AstVisitorFutureAttrs : public AstVisitor {
public:
    AstVisitorFutureAttrs( std::set<Rc_string> &names );

    virtual void enter_destruct_assign( const Vec<Rc_string> &names, const Vec<Rc_string> &equal, const Vec<Rc_string> &types, Rc_string value, PI8 flags ) override;
    virtual void enter_assign         ( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) override;
    virtual void enter_def            ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) override;
    virtual void enter_class          ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) override;
    virtual void enter_enum           ( Rc_string name, const Vec<Rc_string> &items ) override;
    virtual bool enter_test_block     ( const Vec<Rc_string> &items ) override;
    virtual bool enter_test_ss_block  ( const Vec<Rc_string> &items ) override;

    std::set<Rc_string> &names;
};

