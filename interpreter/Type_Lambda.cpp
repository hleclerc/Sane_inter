#include "CallableWithSelfAndArgs.h"
#include "System/Bool_vec.h"
#include "System/rcast.h"
#include "Type_Lambda.h"
#include "Varargs.h"
#include "Scope.h"
#include "TCI.h"
#include "Vm.h"

Type_Lambda::Type_Lambda() : Type_BaseBin( "Lambda" ) {
}

bool Type_Lambda::destroy_attrs() const {
    return false;
}

Variable Type_Lambda::chbeba( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    TODO;
    return {};
}

Variable Type_Lambda::select( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    TODO;
    return {};
}

Variable Type_Lambda::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    Lambda *def = rcast( self.ptr() );

    // scope
    Scope new_scope( Scope::Scope_type::CALL, scope );
    for( const CatchedVariable &cv : def->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

    // support variable for arguments
    Bool_vec defined_args( def->arg_names.size(), false );
    TCI tci;

    // init varargs
    Vec<Varargs *> vpv;

    //    scope->add_error( "pouet" );
    for( size_t i : def->arg_spreads ) {
        Variable vav( scope->vm, scope->vm->type_Varargs );
        new_scope.reg_var( def->arg_names[ i ], vav );
        defined_args.set( i, true );
        vpv << rcast( vav.ptr() );
        // tr_args[ i ] = vav;
    }

    // some basic tests
    if ( def->arg_spreads.size() == 0 ) {
        if ( args.size() > def->arg_names.size() )
            return scope->add_error( "too much argument" ), scope->vm->ref_error;
        for( const Rc_string &name : names )
            if ( ! def->arg_names.contains( name ) )
                return scope->add_error( "name '{}' not present in arguments", name ), scope->vm->ref_error;
    }
    size_t nb_mandatory_args = 0;
    for( size_t i = 0; i < def->arg_names.size(); ++i )
        nb_mandatory_args += def->arg_spreads.contains( i ) == false && ! def->arg_def_vals[ i ];
    if ( args.size() < nb_mandatory_args ) {
        return scope->add_error( "not enough arguments for the lambda call" ), scope->vm->ref_error;
    }

    // unnamed arguments
    for( size_t i = 0, n = 0; i < args.size() - names.size(); ) {
        if ( n == def->arg_names.size() )
            return scope->add_error( def->arg_spreads.empty() ? "too much arguments" : "too much arguments (not fitting in spreads)" ), scope->vm->ref_error;
        int ind_spread = def->arg_spreads.index_first( n );
        if ( ind_spread >= 0 ) {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = new_scope.visit( def->arg_constraints[ n ], true ).ugs( scope );
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ i ].ugs( scope ), tci );
                if ( err.size() ) {
                    ++n;
                    continue;
                }
            }
            vpv[ ind_spread ]->values << args[ i++ ];
        } else {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = new_scope.visit( def->arg_constraints[ n ], true ).ugs( scope );
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ i ].ugs( scope ), tci );
                if ( err.size() )
                    return scope->add_error( "argument '" + def->arg_names[ n ] + "' " + err ), scope->vm->ref_error;
            }
            defined_args.set( n, true );
            new_scope.reg_var( def->arg_names[ n++ ], args[ i++ ] );
            // tr_args[ n++ ] = args[ i++ ];
        }
    }

    // named arguments
    for( size_t i = 0; i < names.size(); ++i ) {
        int ind = def->arg_names.index_first( names[ i ] );
        size_t n = args.size() - names.size() + i;
        // a corresponding arg ?
        if ( ind >= 0 && ! def->arg_spreads.contains( size_t( ind ) ) ) {
            if ( def->arg_constraints[ ind ] ) {
                Variable constraint = new_scope.visit( def->arg_constraints[ ind ], true ).ugs( scope );
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ n ].ugs( scope ), tci );
                if ( err.size() )
                    return scope->add_error( "argument '" + names[ i ] + "' " + err ), scope->vm->ref_error;
            }
            defined_args.set( ind, true );
            new_scope.reg_var( def->arg_names[ ind ], args[ n ] );
            // tr_args[ ind ] = args[ n ];
        } else if ( def->arg_spreads.size() ) {
            // a varargs
            std::string errors;
            for( size_t j = 0;; ++j ) {
                if ( j == def->arg_spreads.size() )
                    return scope->add_error( "argument '" + names[ i ] + "' " + errors ), scope->vm->ref_error;
                if ( const Ast_crepr &cst = def->arg_constraints[ def->arg_spreads[ j ] ] ) {
                    Variable constraint = new_scope.visit( cst, true ).ugs( scope );
                    std::string err = constraint.valid_constraint_for( &new_scope, args[ n ].ugs( scope ), tci );
                    if ( err.size() ) {
                        if ( errors.size() )
                            errors += " and ";
                        errors += err;
                        continue;
                    }
                }
                vpv[ j ]->values << args [ n ];
                vpv[ j ]->names  << names[ i ];
                break;
            }
        } else
            return scope->add_error( "there's no argument named '" + names[ i ] + "'" ), scope->vm->ref_error;
    }

    // not defined arguments
    for( size_t i = 0; i < def->arg_names.size(); ++i ) {
        if ( defined_args.get( i ) == false ) {
            if ( ! def->arg_def_vals[ i ] )
                return scope->add_error( "argument '" + def->arg_names[ i ] + "' is not defined" ), scope->vm->ref_error;
            Variable var = new_scope.visit( def->arg_def_vals[ i ], true );
            new_scope.reg_var( def->arg_names[ i ], var );
            // tr_args[ i ] = var;
        }
    }

    // self as first argument
    if ( def->arg_names.size() && def->arg_names[ 0 ] == "self" )
        new_scope.self = new_scope.find_variable( "self" );

    // __arguments
    new_scope.__arguments_names = def->arg_names;

    // execution
    return new_scope.visit( def->body, want_ret );
}

Variable Type_Lambda::with_self( Scope *scope, const Variable &orig, const Variable &new_self ) {
    Lambda *l = rcast( orig.ptr() );
    if ( l->arg_names.empty() || l->arg_names[ 0 ] != "self" )
        return orig;

    Variable res( scope->vm, scope->vm->type_CallableWithSelfAndArgs );
    CallableWithSelfAndArgs *c = rcast( res.ptr() );
    c->callable = orig;
    c->self = new_self;
    return res;
}

