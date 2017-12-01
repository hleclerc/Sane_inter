#include "System/Bool_vec.h"
#include "AnonymousRoom.h"
#include "System/rcast.h"
#include "System/Math.h"
#include "SlTrialClass.h"
#include "SurdefList.h"
#include "Type_Class.h"
#include "Type_Type.h"
#include "Wildcard.h"
#include "Varargs.h"
#include "Scope.h"
#include "equal.h"
#include "Def.h"
#include "Vm.h"
#include <cmath>

bool same_args( Scope *scope, const Vec<Variable *> &a, const Vec<Variable> &b ) {
    if ( a.size() != b.size() )
        return false;
    for( size_t i = 0; i < a.size(); ++i )
        if ( ! equal( scope, *a[ i ], b[ i ].ugs( scope ) ) )
            return false;
    return true;
}

Type *type_for_args( Scope *scope, Class *def, const Variable &def_var, const Vec<Variable> &args ) {
    if ( args.size() == 1 && args[ 0 ].type == scope->vm->type_Wildcard )
        scope->add_error( "cds" );

    // existing type ?
    for( Type *type : def->instances )
        if ( same_args( scope, type->parameters, args ) )
            return type;

    // else, make a new one
    Type *res = scope->vm->type_ptr_for( def->name, args );
    Scope new_scope( Scope::Scope_type::TYPE_CTOR, scope );
    PI32 old_size = res->size;
    def->instances << res;
    res->orig = scope->vm->main_scope.add_static_variable( def_var );

    // template_parameters in scope
    for( size_t i = 0; i < args.size(); ++i )
        new_scope.reg_var( def->arg_names[ i ], args[ i ], Scope::VariableFlags::TEMPLATE );
    for( const CatchedVariable &cv : def->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

    // inheritance
    for( size_t n = 0; n < def->inheritance.size(); ++n ) {
        // catched variables
        Scope inh_scope( Scope::Scope_type::BLOCK, &new_scope );
        for( const CatchedVariable &cv : def->catched_variables_inh[ n ] )
            inh_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

        // execution
        Variable inh_ref = inh_scope.visit( def->inheritance[ n ], true );
        inh_ref = inh_ref.apply( &inh_scope, true, {}, {}, false );
        new_scope.reg_var( def->inheritance_names[ n ], inh_ref, Scope::VariableFlags::SUPER );

        // abstract methods
        for( const FunctionSignature &fs : inh_ref.type->abstract_methods )
            res->abstract_methods.insert( fs );
    }

    // block exec
    new_scope.in_construction = res;
    new_scope.visit( def->block, false );

    // get variables in a vec, to sweep them in inverse chronological ordering
    Vec<Scope::NV *> variables( Rese(), new_scope.variables.size() );
    for( Scope::NV *nv = new_scope.last_var; nv; nv = nv->prev )
        variables << nv;

    // store type data
    SI32 kv_size = 0;
    SI32 kv_alig = 1;

    // if we have a virtual method, but no vtable from the first parent, we need room for a new vtable
    auto needs_new_vtable = [&]() {
        for( const Rc_string &name : def->inheritance_names ) {
            Type *inh_type = new_scope.find_variable( name ).type;
            if ( inh_type->has_vtable_at_the_beginning() )
                return false;
            if ( inh_type->size )
                break;
        }
        for( Scope::NV *m : variables ) {
            if ( m->flags & Scope::VariableFlags::CALLABLE && m->var.type == scope->vm->type_SurdefList ) {
                SurdefList *sl = rcast( m->var.ptr() );
                for( const Variable &sv : sl->lst ) {
                    if ( sv.type == scope->vm->type_Def ) {
                        Def *def = rcast( sv.ptr() );
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
        res->has_new_vtable = true;
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
            if ( m.var.type == scope->vm->type_SurdefList ) {
                SurdefList *sl = rcast( m.var.ptr() );

                // abstract methods
                bool has_defs = false;
                for( const Variable &v : sl->lst ) {
                    if ( v.type == scope->vm->type_Def ) {
                        Def *def = rcast( v.ptr() );
                        has_defs = true;
                        if ( def->abstract_flag )
                            res->abstract_methods.insert( def );
                    }
                }

                //
                if ( has_defs ) {
                    res->methods[ m.name ] = scope->vm->main_scope.add_static_variable( m.var );
                    continue;
                }
            } else
                TODO;
        }
        if ( m.flags & Scope::VariableFlags::STATIC ) {
            res->static_attributes[ m.name ] = scope->vm->main_scope.add_static_variable( m.var );
            continue;
        }

        // anonymous attribute (room reservation)
        if ( m.var.type == scope->vm->type_AnonymousRoom && kv_size >= 0 && kv_alig >= 0 ) {
            AnonymousRoom *ar = rcast( m.var.ptr() );
            kv_size = ceil( kv_size, ar->alig ) + ar->size;
            kv_alig = lcm ( kv_alig, ar->alig );
            continue;
        }

        // else, look if we have fixed size information from the types
        int attr_kv_size = m.var.type->size;
        int attr_kv_alig = m.var.type->alig;
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
        res->size = kv_size;
        res->alig = kv_alig;
    }

    // preparation of vtables
    res->apply_on_vtables( [&]( Type *type, SI32 off ) {
        Vtable *vtable = res->vtables.new_back_item();
        vtable->offsetB = off;
        vtable->type    = res;
    } );

    return res;
}

Type_Class::Type_Class() : Type_BaseBin( "Class" ) {
}

FP64 Type_Class::get_pertinence( Scope *scope, const Variable &self ) const {
    Class *cl = reinterpret_cast<Class *>( self.ptr() );
    return cl->def_pert_num * std::pow( 10.0, cl->def_pert_exp );
}


Variable Type_Class::make_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &args, const Vec<Rc_string> &names, const Vec<Variable> &ctor_args, const Vec<Rc_string> &ctor_names, bool call_ctor ) const {
    Class *def = rcast( func.ptr() );

    //
    Bool_vec defined_args( def->arg_names.size(), false );

    // make a Sl_trial_Def
    Variable tr_var( scope->vm, scope->vm->type_SlTrialClass );
    SlTrialClass *tr = rcast( tr_var.ptr() );
    tr->args.resize( def->arg_names.size() );
    tr->def = func;

    // new scope, to compute condition, etc...
    Scope new_scope( Scope::Scope_type::CALL, scope );

    // catched variables for defaults, condition, ...
    for( size_t i = 0, s = def->catched_variables_prep.size(); i < s; ++i )
        new_scope.reg_var( def->catched_variables_prep[ i ].name, def->catched_variables_prep[ i ].val, Scope::VariableFlags::CATCHED );
    for( const Rc_string &name : def->with_names ) {
        Variable wc( scope->vm, scope->vm->type_Wildcard );
        reinterpret_cast<Wildcard *>( wc.ptr() )->name = name;
        new_scope.reg_var( name, wc, Scope::VariableFlags::TEMPLATE );
    }
    // new_scope.valid_scope_ptr = def->valid_scope_ptr.get_scope();

    // init varargs
    Vec<Varargs *> vpv;
    for( size_t i : def->arg_spreads ) {
        Variable vav( scope->vm, scope->vm->type_Varargs );
        new_scope.reg_var( def->arg_names[ i ], vav );
        defined_args.set( i, true );
        vpv << rcast( vav.ptr() );
        tr->args[ i ] = vav;
    }

    // function to be called if fail
    auto fail = [&]( const Rc_string &reason ) {
        tr->condition = false;
        tr->msg       = reason;
        return tr_var;
    };

    // some basic tests
    if ( def->arg_spreads.size() == 0 ) {
        if ( args.size() - names.size() > def->arg_names.size() )
            return fail( "too much argument" );
        for( const Rc_string &name : names )
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
                Variable constraint = new_scope.visit( def->arg_constraints[ n ], true ).ugs( scope );
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ i ].ugs( scope ), tr->tci );
                if ( err.size() ) {
                    ++n;
                    continue;
                }
            }
            vpv[ ind_spread ]->values << args[ i++ ];
        } else {
            if ( def->arg_constraints[ n ] ) {
                Variable constraint = new_scope.visit( def->arg_constraints[ n ], true ).ugs( scope );
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ i ].ugs( scope ), tr->tci );
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
                Variable constraint = new_scope.visit( def->arg_constraints[ ind ], true ).ugs( scope );
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ i ].ugs( scope ), tr->tci );
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
                if ( const Ast_crepr &cst = def->arg_constraints[ def->arg_spreads[ j ] ] ) {
                    Variable constraint = new_scope.visit( cst, true ).ugs( scope );
                    std::string err = constraint.valid_constraint_for( &new_scope, args[ n ].ugs( scope ), tr->tci );
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
            Variable var = new_scope.visit( def->arg_def_vals[ i ], true );
            new_scope.reg_var( def->arg_names[ i ], var );
            tr->args[ i ] = var;
        }
    }

    // condition
    if ( Ast_crepr &cond = def->condition ) {
        // a new scope with solved wildcards
        Scope new_new_scope( Scope::Scope_type::BLOCK, &new_scope );
        for( const auto &v : tr->tci.proposals )
            new_new_scope.reg_var( v.first, v.second );

        // compute the condition
        Variable cond_ref = new_new_scope.visit( cond, true );

        // convert to bool
        if ( cond_ref.type != scope->vm->type_Bool ) {
            Variable cond_func = new_new_scope.find_variable( "Bool" );
            if ( cond_func.error() )
                return new_new_scope.add_error( "Impossible to find class 'Bool'" ), fail( "internal error" );
            Variable res = cond_func.apply( &new_new_scope, true, cond_ref );
            cond_ref = res;
        }
        Bool *cond_val = rcast( cond_ref.ptr() );
        if ( ! *cond_val )
            return fail( "condition not met" );
    }

    //
    tr->condition = true;
    return tr_var;
}

Variable Type_Class::use_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &sl_var, const Vec<Variable> &args, const Vec<Rc_string> &names, const Vec<Variable> &ctor_args, const Vec<Rc_string> &ctor_names, bool call_ctor, const Variable &trial ) const {
    Class *def = rcast( func.ptr() );
    SlTrialClass *tr = rcast( trial.ptr() );
    ASSERT( tr->tci.proposals.empty(), "TODO" );
    return Type_Type::make_inst( scope, type_for_args( scope, def, func, tr->args ), ctor_args, ctor_names, call_ctor );
}

