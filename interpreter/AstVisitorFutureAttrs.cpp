#include "AstVisitorFutureAttrs.h"


AstVisitorFutureAttrs::AstVisitorFutureAttrs( std::set<Rc_string> &names ) : names( names ) {
    rec = false;
}

void AstVisitorFutureAttrs::enter_destruct_assign( const Vec<Rc_string> &names, const Vec<Rc_string> &equal, const Vec<Rc_string> &types, Rc_string value, PI8 flags ) {
    if ( flags & ASSIGN_FLAG_global )
        return;
    for( const Rc_string &name : names )
        this->names.insert( name );
}

void AstVisitorFutureAttrs::enter_assign( Rc_string name, Rc_string cname, PI8 nb_scopes_res, Rc_string value, PI8 flags ) {
    if ( flags & ASSIGN_FLAG_global )
        return;
    names.insert( name );
}

void AstVisitorFutureAttrs::enter_def( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, Rc_string self_constraint, Rc_string return_type, const Vec<Rc_string> &wpc ) {
    if ( flags & CALLABLE_FLAG_global )
        return;
    if ( name )
        names.insert( name );
}

void AstVisitorFutureAttrs::enter_class( Rc_string name, Rc_string cname, PI8 nb_scopes_rec, const Vec<Rc_string> &arg_names, const Vec<Rc_string> &arg_constraints, const Vec<Rc_string> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<Rc_string> &with_names, const Vec<Rc_string> &with_constraints, const Vec<Rc_string> &with_def_vals, const Vec<size_t> &with_spreads, Rc_string condition, Rc_string pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<Rc_string> &block, const Vec<Rc_string> &inheritance_names, const Vec<Rc_string> &inheritance ) {
    if ( flags & CALLABLE_FLAG_global )
        return;
    if ( name )
        names.insert( name );
}

void AstVisitorFutureAttrs::enter_enum( Rc_string name, const Vec<Rc_string> &items ) {
    names.insert( name );
}

bool AstVisitorFutureAttrs::enter_test_block( const Vec<Rc_string> &items ) {
    return true;
}

bool AstVisitorFutureAttrs::enter_test_ss_block( const Vec<Rc_string> &items ) {
    for( const Rc_string &item : items )
        ast_visit( *this, item );
    return true;
}

