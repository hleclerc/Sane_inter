#include "CallableWithSelf.h"
#include "System/BoolVec.h"
#include "System/rcast.h"
#include "TypeLambda.h"
#include "Varargs.h"
#include "Lambda.h"
#include "Scope.h"
#include "TCI.h"
#include "gvm.h"

TypeLambda::TypeLambda() : Type( "Lambda" ) {
}

bool TypeLambda::destroy_attrs() const {
    return false;
}

Variable TypeLambda::chbeba( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    TODO;
    return {};
}

Variable TypeLambda::select( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    TODO;
    return {};
}

Variable TypeLambda::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) {
    Lambda *def = self.rcast<Lambda>();

    // scope
    Scope new_scope( Scope::ScopeType::CALL );
    for( const CatchedVariable &cv : def->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

    // support variable for arguments
    BoolVec defined_args( def->arg_names.size(), false );
    TCI tci;

    // init varargs
    Vec<Varargs *> vpv;

    //    gvm->add_error( "pouet" );
    for( size_t i : def->arg_spreads ) {
        Variable vav( MAKE_KV( Varargs ) );
        new_scope.reg_var( def->arg_names[ i ], vav );
        defined_args.set( i, true );
        vpv << vav.rcast<Varargs>();
        // tr_args[ i ] = vav;
    }

    // some basic tests
    if ( def->arg_spreads.size() == 0 ) {
        if ( args.size() > def->arg_names.size() )
            return gvm->add_error( "too much argument" );
        for( const RcString &name : names )
            if ( ! def->arg_names.contains( name ) )
                return gvm->add_error( "name '{}' not present in arguments", name );
    }
    size_t nb_mandatory_args = 0;
    for( size_t i = 0; i < def->arg_names.size(); ++i )
        nb_mandatory_args += def->arg_spreads.contains( i ) == false && ! def->arg_def_vals[ i ];
    if ( args.size() < nb_mandatory_args ) {
        return gvm->add_error( "not enough arguments for the lambda call" );
    }

    // unnamed arguments
    for( size_t i = 0, n = 0; i < args.size() - names.size(); ) {
        if ( n == def->arg_names.size() )
            return gvm->add_error( def->arg_spreads.empty() ? "too much arguments" : "too much arguments (not fitting in spreads)" );
        int ind_spread = def->arg_spreads.index_first( n );
        if ( ind_spread >= 0 ) {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = gvm->visit( def->arg_constraints[ n ], true );
                RcString err = constraint.valid_constraint_for( args[ i ], tci );
                if ( err.size() ) {
                    ++n;
                    continue;
                }
            }
            vpv[ ind_spread ]->values << args[ i++ ];
        } else {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = gvm->visit( def->arg_constraints[ n ], true );
                RcString err = constraint.valid_constraint_for( args[ i ], tci );
                if ( err.size() )
                    return gvm->add_error( "argument '" + def->arg_names[ n ] + "' " + err );
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
                Variable constraint = gvm->visit( def->arg_constraints[ ind ], true );
                RcString err = constraint.valid_constraint_for( args[ n ], tci );
                if ( err.size() )
                    return gvm->add_error( "argument '" + names[ i ] + "' " + err );
            }
            defined_args.set( ind, true );
            new_scope.reg_var( def->arg_names[ ind ], args[ n ] );
            // tr_args[ ind ] = args[ n ];
        } else if ( def->arg_spreads.size() ) {
            // a varargs
            std::string errors;
            for( size_t j = 0;; ++j ) {
                if ( j == def->arg_spreads.size() )
                    return gvm->add_error( "argument '" + names[ i ] + "' " + errors );
                if ( const AstCrepr &cst = def->arg_constraints[ def->arg_spreads[ j ] ] ) {
                    Variable constraint = gvm->visit( cst, true );
                    std::string err = constraint.valid_constraint_for( args[ n ], tci );
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
            return gvm->add_error( "there's no argument named '{}'", names[ i ] );
    }

    // not defined arguments
    for( size_t i = 0; i < def->arg_names.size(); ++i ) {
        if ( defined_args.get( i ) == false ) {
            if ( ! def->arg_def_vals[ i ] )
                return gvm->add_error( "argument '{}' is not defined", def->arg_names[ i ] );
            Variable var = gvm->visit( def->arg_def_vals[ i ], true );
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
    return gvm->visit( def->body, want_ret );
}

Variable TypeLambda::with_self( Variable &orig, const Variable &new_self ) const {
    Lambda *l = orig.rcast<Lambda>();
    if ( l->arg_names.empty() || l->arg_names[ 0 ] != "self" )
        return orig;

    Variable res( MAKE_KV( CallableWithSelf ) );
    CallableWithSelf *c = res.rcast<CallableWithSelf>();
    c->callable = orig;
    c->self = new_self;
    return res;
}



