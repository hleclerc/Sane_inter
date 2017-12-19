#include "FunctionSignature.h"
#include "System/RaiiSave.h"
#include "System/BoolVec.h"
#include "AnonymousRoom.h"
#include "SlTrialClass.h"
#include "System/Math.h"
#include "SurdefList.h"
#include "TypeClass.h"
#include "Wildcard.h"
#include "Varargs.h"
#include "Class.h"
#include "Scope.h"
#include "equal.h"
#include "Def.h"
#include "gvm.h"
#include <cmath>

bool same_args( const Vec<Variable *> &a, const Vec<Variable> &b ) {
    if ( a.size() != b.size() )
        return false;
    for( size_t i = 0; i < a.size(); ++i )
        if ( ! equal( *a[ i ], b[ i ] ) )
            return false;
    return true;
}

Type *type_for_args( Class *def, const Variable &def_var, const Vec<Variable> &args ) {
    // existing type ?
    for( Type *type : def->instances )
        if ( same_args( type->content.data.parameters, args ) )
            return type;

    // else, make a new one
    Type *res = gvm->type_ptr_for( def->name, args );
    Scope new_scope( Scope::ScopeType::TYPE_CTOR );
    PI32 old_size = res->content.data.size;
    res->content.data.orig_class = def;
    def->instances << res;

    // template_parameters in scope
    for( size_t i = 0; i < args.size(); ++i )
        new_scope.reg_var( def->arg_names[ i ], args[ i ], Scope::VariableFlags::TEMPLATE );
    for( const CatchedVariable &cv : def->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

    // inheritance
    for( size_t n = 0; n < def->inheritance.size(); ++n ) {
        // catched variables
        Scope inh_scope( Scope::ScopeType::BLOCK );
        for( const CatchedVariable &cv : def->catched_variables_inh[ n ] )
            inh_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

        // execution
        Variable inh_ref = gvm->visit( def->inheritance[ n ], true );
        inh_ref = inh_ref.apply( true, {}, {}, ApplyFlags::DONT_CALL_CTOR );
        new_scope.reg_var( def->inheritance_names[ n ], inh_ref, Scope::VariableFlags::SUPER );

        // abstract methods
        for( const FunctionSignature &fs : inh_ref.type->content.data.abstract_methods )
            res->content.data.abstract_methods.insert( fs );
    }

    // block exec
    new_scope.in_construction = res;
    gvm->visit( def->block, false );

    // get variables in a vec, to sweep them in inverse chronological ordering
    Vec<Scope::NV *> variables( Rese(), new_scope.variables.size() );
    for( Scope::NV *nv = new_scope.last_var; nv; nv = nv->prev )
        variables << nv;

    // store type data
    SI32 kv_size = 0;
    SI32 kv_alig = 1;

    // if we have a virtual method, but no vtable from the first parent, we need room for a new vtable
    auto needs_new_vtable = [&]() {
        for( const RcString &name : def->inheritance_names ) {
            Type *inh_type = new_scope.find_variable( name ).type;
            if ( inh_type->has_vtable_at_the_beginning() )
                return false;
            if ( inh_type->content.data.size )
                break;
        }
        for( Scope::NV *m : variables ) {
            if ( m->flags & Scope::VariableFlags::CALLABLE && m->var.type == gvm->type_SurdefList ) {
                SurdefList *sl = m->var.rcast<SurdefList>();
                for( const Variable &sv : sl->lst ) {
                    if ( sv.type == gvm->type_Def ) {
                        Def *def = sv.rcast<Def>();
                        if ( def->virtual_flag )
                            return true;
                    }
                }
            }
        }
        return false;
    };
    // we already have this room ??
    if ( needs_new_vtable() ) {
        res->content.data.has_new_vtable = true;
        kv_size = 8 * sizeof( void * );
        kv_alig = 8 * sizeof( void * );
    }

    //
    for( size_t num_var = variables.size(); num_var--; ) {
        // static attributes
        Scope::NV &m = *variables[ num_var ];
        if ( m.flags & Scope::VariableFlags::TEMPLATE )
            continue;
        if ( m.flags & Scope::VariableFlags::CATCHED )
            continue;
        if ( m.flags & Scope::VariableFlags::CALLABLE ) {
            if ( m.var.type == gvm->type_SurdefList ) {
                SurdefList *sl = m.var.rcast<SurdefList>();

                // abstract methods
                bool has_defs = false;
                for( const Variable &v : sl->lst ) {
                    if ( v.type == gvm->type_Def ) {
                        Def *def = v.rcast<Def>();
                        has_defs = true;
                        if ( def->abstract_flag )
                            res->content.data.abstract_methods.insert( def );
                    }
                }

                //
                if ( has_defs ) {
                    res->content.data.methods[ m.name ] = gvm->main_scope.add_static_variable( m.var );
                    continue;
                }
            } else
                TODO;
        }
        if ( m.flags & Scope::VariableFlags::STATIC ) {
            res->content.data.static_attributes[ m.name ] = gvm->main_scope.add_static_variable( m.var );
            continue;
        }

        // anonymous attribute (room reservation)
        if ( m.var.type == gvm->type_AnonymousRoom && kv_size >= 0 && kv_alig >= 0 ) {
            AnonymousRoom *ar = m.var.rcast<AnonymousRoom>();
            kv_size = ceil( kv_size, ar->alig ) + ar->size;
            kv_alig = lcm ( kv_alig, ar->alig );
            continue;
        }

        // else, look if we have fixed size information from the types
        int attr_kv_size = m.var.type->content.data.size;
        int attr_kv_alig = m.var.type->content.data.alig;
        if ( kv_size >= 0 && kv_alig >= 0 && attr_kv_size >= 0 && attr_kv_alig >= 0 ) {
            kv_alig = lcm ( kv_alig, attr_kv_alig );
            kv_size = ceil( kv_size, attr_kv_alig );

            res->add_attribute( m.name, kv_size, m.var.type );

            kv_size += attr_kv_size;
        } else if ( kv_alig >= 0 && attr_kv_alig >= 0 ) {
            kv_alig = lcm( kv_alig, attr_kv_alig );
            kv_size = -1;

            res->add_attribute( m.name, -1, m.var.type );
        } else {
            res->add_attribute( m.name, -1, m.var.type );
            kv_size = -1;
            kv_alig = -1;
        }
    }

    // if size has not been specified before, we store the result of the computation
    if ( old_size == 0 ) {
        res->content.data.size = kv_size;
        res->content.data.alig = kv_alig;
    }

    // preparation of vtables
    //    res->apply_on_vtables( [&]( Type *type, SI32 off ) {
    //        Vtable *vtable = res->vtables.new_back_item();
    //        vtable->offset = off;
    //        vtable->type   = res;
    //    } );

    return res;
}

TypeClass::TypeClass() : Type( "Def" ) {
}

double TypeClass::get_pertinence( const Variable &self ) const {
    Class *cl = self.rcast<Class>();
    return cl->def_pert_num * std::pow( 10.0, cl->def_pert_exp );
}

Variable TypeClass::make_sl_trial( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) const {
    Class *def = func.rcast<Class>();

    //
    BoolVec defined_args( def->arg_names.size(), false );

    // make a Sl_trial_Def
    Variable tr_var( new KnownRef<SlTrialClass>, gvm->type_SlTrialClass );
    SlTrialClass *tr = tr_var.rcast<SlTrialClass>();
    tr->args.resize( def->arg_names.size() );
    tr->def = func;

    // new scope, to compute condition, etc...
    Scope new_scope( Scope::ScopeType::CALL );

    // catched variables for defaults, condition, ...
    for( size_t i = 0, s = def->catched_variables_prep.size(); i < s; ++i )
        new_scope.reg_var( def->catched_variables_prep[ i ].name, def->catched_variables_prep[ i ].val, Scope::VariableFlags::CATCHED );
    for( const RcString &name : def->with_names ) {
        Variable wc( new KnownRef<Wildcard>, gvm->type_Wildcard );
        wc.rcast<Wildcard>()->name = name;
        new_scope.reg_var( name, wc, Scope::VariableFlags::TEMPLATE );
    }
    // new_scope.valid_scope_ptr = def->valid_scope_ptr.get_scope();

    // init varargs
    Vec<Varargs *> vpv;
    for( size_t i : def->arg_spreads ) {
        Variable vav( new KnownRef<Varargs>, gvm->type_Varargs );
        new_scope.reg_var( def->arg_names[ i ], vav );
        defined_args.set( i, true );
        vpv << vav.rcast<Varargs>();
        tr->args[ i ] = vav;
    }

    // function to be called if fail
    auto fail = [&]( const RcString &reason ) {
        tr->condition = false;
        tr->msg       = reason;
        return tr_var;
    };

    // some basic tests
    if ( def->arg_spreads.size() == 0 ) {
        if ( args.size() - names.size() > def->arg_names.size() )
            return fail( "too much argument" );
        for( const RcString &name : names )
            if ( ! def->arg_names.contains( name ) )
                return fail( "name '" + name + "' not present in arguments" );
    }
    size_t nb_mandatory_args = 0;
    for( size_t i = 0; i < def->arg_names.size(); ++i )
        nb_mandatory_args += def->arg_spreads.contains( i ) == false && ! def->arg_def_vals[ i ];
    if ( args.size() < nb_mandatory_args )
        return fail( "not enough arguments" );

    // unnamed arguments
    for( size_t i = 0, n = 0; i < args.size() - names.size(); ) {
        if ( n == def->arg_names.size() )
            return fail( def->arg_spreads.empty() ? "too much arguments" : "too much arguments (not fitting in spreads)" );
        int ind_spread = def->arg_spreads.index_first( n );
        if ( ind_spread >= 0 ) {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = gvm->visit( def->arg_constraints[ n ], true );
                RcString err = constraint.valid_constraint_for( args[ i ], tr->tci );
                if ( err.size() ) {
                    ++n;
                    continue;
                }
            }
            vpv[ ind_spread ]->values << args[ i++ ];
        } else {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = gvm->visit( def->arg_constraints[ n ], true );
                RcString err = constraint.valid_constraint_for( args[ i ], tr->tci );
                if ( err.size() )
                    return fail( "argument '" + def->arg_names[ n ] + "' " + err );
            }
            defined_args.set( n, true );
            new_scope.reg_var( def->arg_names[ n ], args[ i ] );
            tr->args[ n++ ] = args[ i++ ];
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
                RcString err = constraint.valid_constraint_for( args[ i ], tr->tci );
                if ( err.size() )
                    return fail( "argument '" + names[ i ] + "' " + err );
            }
            defined_args.set( ind, true );
            new_scope.reg_var( def->arg_names[ ind ], args[ n ] );
            tr->args[ ind ] = args[ n ];
        } else if ( def->arg_spreads.size() ) {
            // a varargs
            std::string errors;
            for( size_t j = 0;; ++j ) {
                if ( j == def->arg_spreads.size() )
                    return fail( "argument '" + names[ i ] + "' " + errors );
                if ( const AstCrepr &cst = def->arg_constraints[ def->arg_spreads[ j ] ] ) {
                    Variable constraint = gvm->visit( cst, true );
                    std::string err = constraint.valid_constraint_for( args[ n ], tr->tci );
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
            return fail( "there's no argument named '" + names[ i ] + "'" );
    }

    // not defined arguments
    for( size_t i = 0; i < def->arg_names.size(); ++i ) {
        if ( defined_args.get( i ) == false ) {
            if ( ! def->arg_def_vals[ i ] )
                return fail( "argument '" + def->arg_names[ i ] + "' is not defined" );
            Variable var = gvm->visit( def->arg_def_vals[ i ], true );
            new_scope.reg_var( def->arg_names[ i ], var );
            tr->args[ i ] = var;
        }
    }

    // condition
    if ( AstCrepr &cond = def->condition ) {
        // a new scope with solved wildcards
        Scope new_new_scope( Scope::ScopeType::BLOCK );
        for( const auto &v : tr->tci.proposals )
            new_new_scope.reg_var( v.first, v.second );

        // compute the condition
        Variable cond_ref = gvm->visit( cond, true );

        // convert to bool
        if ( cond_ref.type != gvm->type_Bool ) {
            Variable cond_func = new_new_scope.find_variable( "Bool" );
            if ( cond_func.error() )
                return gvm->add_error( "Impossible to find class 'Bool'" ), fail( "internal error" );
            Variable res = cond_func.apply( true, cond_ref );
            cond_ref = res;
        }
        Bool *cond_val = cond_ref.rcast<Bool>();
        if ( ! *cond_val )
            return fail( "condition not met" );
    }

    //
    tr->condition = true;
    return tr_var;
}

Variable TypeClass::use_sl_trial( bool want_ret, const Variable &func, const Variable &sl_var, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &ctor_args, const Vec<RcString> &ctor_names, const Variable &with_self, ApplyFlags apply_flags, const Variable &trial ) const {
    Class *def = func.rcast<Class>();
    SlTrialClass *tr = trial.rcast<SlTrialClass>();
    ASSERT( tr->tci.proposals.empty(), "TODO" );
    return gvm->make_inst( type_for_args( def, func, tr->args ), ctor_args, ctor_names, apply_flags );
}

