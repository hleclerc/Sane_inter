#include "AstVisitorCatchedVariables.h"

/**
*/
class AstVisitorPreCatchedVariables : public AstVisitor {
public:
    AstVisitorPreCatchedVariables( AstVisitorCatchedVariables *ac ) : ac( ac ) { rec = false; }

    virtual void enter_destruct_assign( const Vec<RcString> &names, const Vec<RcString> &equal, const Vec<RcString> &types, RcString value, PI8 flags ) override {
        for( const RcString &name : names )
            ac->new_var( name );
    }
    virtual void enter_assign( RcString name, RcString cname, PI8 nb_scopes_res, RcString value, PI8 flags ) override {
        ac->new_var( name );
    }
    virtual void enter_def   ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, RcString self_constraint, RcString return_type, const Vec<RcString> &wpc ) override {
        if ( name )
            ac->new_var( name );
    }
    virtual void enter_class ( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance ) override {
        if ( name )
            ac->new_var( name );
    }

    virtual bool enter_test_block( const Vec<RcString> &items ) override {
        return true;
    }

    virtual bool enter_test_ss_block( const Vec<RcString> &items ) override {
        for( const RcString &item : items )
            ast_visit( *this, item );
        return true;
    }

    AstVisitorCatchedVariables *ac;
};


AstVisitorCatchedVariables::AstVisitorCatchedVariables() {
    local.emplace();
}

void AstVisitorCatchedVariables::pre_visit( RcString cm ) {
    AstVisitorPreCatchedVariables ap( this );
    ast_visit( ap, cm );
}

void AstVisitorCatchedVariables::enter_variable( RcString name ) {
    if ( internal.count( name ) )
        return;
    catched.insert( name );
}

void AstVisitorCatchedVariables::enter_block( const Vec<RcString> &items ) {
    new_scope();
}

void AstVisitorCatchedVariables::leave_block() {
    rem_scope();
}

void AstVisitorCatchedVariables::enter_assign( RcString name, RcString cname, PI8 nb_scopes_res, RcString value, PI8 flags ) {
    new_var( name );
}

void AstVisitorCatchedVariables::enter_def( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, RcString self_constraint, RcString return_type, const Vec<RcString> &wpc ) {
    if ( name )
        new_var( name );
    new_scope();
    for( const RcString &arg : arg_names )
        new_var( arg );
    for( const RcString &arg : with_names )
        new_var( arg );
}

void AstVisitorCatchedVariables::leave_def() {
    rem_scope();
}

void AstVisitorCatchedVariables::enter_class( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance ) {
    if ( name )
        new_var( name );
    new_scope();
    for( const RcString &arg : arg_names )
        new_var( arg );
    for( const RcString &arg : with_names )
        new_var( arg );

    // pre-visit attributes
    AstVisitorPreCatchedVariables ap( this );
    for( const RcString &cm : block )
        ast_visit( ap, cm );
}

void AstVisitorCatchedVariables::leave_class() {
    rem_scope();
}

void AstVisitorCatchedVariables::enter_enum( RcString name, const Vec<RcString> &items ) {
    new_var( name );
}

void AstVisitorCatchedVariables::enter_for( RcString name, RcString container, const Vec<RcString> &block ) {
    new_scope();
    if ( name )
        new_var( name );
}

void AstVisitorCatchedVariables::leave_for(){
    rem_scope();
}

void AstVisitorCatchedVariables::enter_lambda( const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &splats, RcString body ) {
    new_scope();
    for( const RcString &arg : arg_names )
        new_var( arg );
}

void AstVisitorCatchedVariables::leave_lambda() {
    rem_scope();
}

void AstVisitorCatchedVariables::enter_try_catch( RcString try_body, RcString arg_name, RcString arg_constraint, RcString catch_body, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_splats ) {
    new_scope();
    new_var( arg_name );
    for( const RcString &arg : with_names )
        new_var( arg );
}

void AstVisitorCatchedVariables::leave_try_catch() {
    rem_scope();
}

void AstVisitorCatchedVariables::new_var( RcString name ) {
    if ( internal.insert( name ).second )
        local.top() << name;
}

void AstVisitorCatchedVariables::new_scope() {
    local.emplace();
}

void AstVisitorCatchedVariables::rem_scope() {
    for( const std::string &name : local.top() )
        internal.erase( name );
    local.pop();
}
