#include "Ast_visitor_catched_variables.h"

/**
*/
class AstVisitorPreCatchedVariables : public AstVisitor {
public:
    AstVisitorPreCatchedVariables( Ast_visitor_catched_variables *ac ) : ac( ac ) { rec = false; }

    virtual void enter_destruct_assign( const Vec<Rc_string> &names, const Vec<Rc_string> &equal, const Vec<Rc_string> &types, Rc_string value, PI8 flags ) override {
        for( const Rc_string &name : names )
            ac->new_var( name );
    }
    virtual void enter_assign( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) override {
        ac->new_var( name );
    }
    virtual void enter_def   ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) override {
        if ( name )
            ac->new_var( name );
    }
    virtual void enter_class ( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) override {
        if ( name )
            ac->new_var( name );
    }

    virtual bool enter_test_block( const Vec<Rc_string> &items ) override {
        return true;
    }

    virtual bool enter_test_ss_block( const Vec<Rc_string> &items ) override {
        for( const Rc_string &item : items )
            ast_visit( *this, item );
        return true;
    }

    Ast_visitor_catched_variables *ac;
};


Ast_visitor_catched_variables::Ast_visitor_catched_variables() {
    local.emplace();
}

void Ast_visitor_catched_variables::pre_visit( Rc_string cm ) {
    AstVisitorPreCatchedVariables ap( this );
    ast_visit( ap, cm );
}

void Ast_visitor_catched_variables::enter_variable( Rc_string name ) {
    if ( internal.count( name ) )
        return;
    catched.insert( name );
}

void Ast_visitor_catched_variables::enter_block( const Vec<Rc_string> &items ) {
    new_scope();
}

void Ast_visitor_catched_variables::leave_block() {
    rem_scope();
}

void Ast_visitor_catched_variables::enter_assign( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) {
    new_var( name );
}

void Ast_visitor_catched_variables::enter_def( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) {
    if ( name )
        new_var( name );
    new_scope();
    for( const Rc_string &arg : arg_names )
        new_var( arg );
    for( const Rc_string &arg : with_names )
        new_var( arg );
}

void Ast_visitor_catched_variables::leave_def() {
    rem_scope();
}

void Ast_visitor_catched_variables::enter_class( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) {
    if ( name )
        new_var( name );
    new_scope();
    for( const Rc_string &arg : arg_names )
        new_var( arg );
    for( const Rc_string &arg : with_names )
        new_var( arg );

    // pre-visit attributes
    AstVisitorPreCatchedVariables ap( this );
    for( const Rc_string &cm : block )
        ast_visit( ap, cm );
}

void Ast_visitor_catched_variables::leave_class() {
    rem_scope();
}

void Ast_visitor_catched_variables::enter_enum( Rc_string name, const Vec<Rc_string> &items ) {
    new_var( name );
}

void Ast_visitor_catched_variables::enter_for( Rc_string name, Rc_string container, const Vec<Rc_string> &block ) {
    new_scope();
    if ( name )
        new_var( name );
}

void Ast_visitor_catched_variables::leave_for(){
    rem_scope();
}

void Ast_visitor_catched_variables::enter_lambda( const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &splats, Rc_string body ) {
    new_scope();
    for( const Rc_string &arg : arg_names )
        new_var( arg );
}

void Ast_visitor_catched_variables::leave_lambda() {
    rem_scope();
}

void Ast_visitor_catched_variables::enter_try_catch( Rc_string try_body, Rc_string arg_name, Rc_string arg_constraint, Rc_string catch_body, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_splats ) {
    new_scope();
    new_var( arg_name );
    for( const Rc_string &arg : with_names )
        new_var( arg );
}

void Ast_visitor_catched_variables::leave_try_catch() {
    rem_scope();
}

void Ast_visitor_catched_variables::new_var( Rc_string name ) {
    if ( internal.insert( name ).second )
        local.top() << name;
}

void Ast_visitor_catched_variables::new_scope() {
    local.emplace();
}

void Ast_visitor_catched_variables::rem_scope() {
    for( const std::string &name : local.top() )
        internal.erase( name );
    local.pop();
}
