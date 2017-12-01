#pragma once

#include "Ast/Ast_visitor.h"
#include <stack>
#include <set>

/**
*/
class Ast_visitor_catched_variables : public AstVisitor {
public:
    Ast_visitor_catched_variables();

    void         pre_visit          ( Rc_string cm );

    virtual void enter_variable     ( Rc_string name ) override;
    virtual void enter_block        ( const Vec<Rc_string> &items ) override;
    virtual void leave_block        () override;
    virtual void enter_assign       ( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) override;
    virtual void enter_def          ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) override;
    virtual void leave_def          () override;
    virtual void enter_class        ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) override;
    virtual void leave_class        () override;
    virtual void enter_enum         ( Rc_string name, const Vec<Rc_string> &items ) override;
    virtual void enter_for          ( Rc_string name, Rc_string container, const Vec<Rc_string> &block ) override;
    virtual void leave_for          () override;
    virtual void enter_lambda       ( const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &splats, Rc_string body ) override;
    virtual void leave_lambda       () override;
    virtual void enter_try_catch    ( Rc_string try_body, Rc_string arg_name, Rc_string arg_constraint, Rc_string catch_body, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_splats );
    virtual void leave_try_catch    ();
    //    virtual void default_enter( const char *name ) { P( "enter", name ); }
    //    virtual void default_leave( const char *name ) { P( "leave", name ); }

    void         new_var            ( Rc_string name );
    void         new_scope          ();
    void         rem_scope          ();

    std::set<Rc_string>        internal;
    std::set<Rc_string>        catched;
    std::stack<Vec<Rc_string>> local;
};
