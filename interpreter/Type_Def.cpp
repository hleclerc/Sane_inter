#include "System/Raii_save.h"
#include "System/Bool_vec.h"
#include "System/rcast.h"
#include "SlTrialDef.h"
#include "SurdefList.h"
#include "Type_Def.h"
#include "Wildcard.h"
#include "Varargs.h"
#include "Class.h"
#include "Vm.h"
#include <cmath>

Type_Def::Type_Def() : Type_BaseBin( "Def" ) {
}

FP64 Type_Def::get_pertinence( Scope *scope, const Variable &self ) const {
    Def *cl = reinterpret_cast<Def *>( self.ptr() );
    return cl->def_pert_num * std::pow( 10.0, cl->def_pert_exp );
}

Variable Type_Def::make_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const {
    if ( sl_args.size() ) {
        Vec<Variable>  n_args ( Rese(), sl_args .size() + args .size() );
        Vec<Rc_string> n_names( Rese(), sl_names.size() + names.size() );
        for( size_t i = 0; i < sl_args.size() - sl_names.size(); ++i )
            n_args << sl_args[ i ];
        for( size_t i = 0; i < args.size() - names.size(); ++i )
            n_args << args[ i ];
        for( size_t i = 0; i < sl_names.size(); ++i ) {
            n_args  << sl_args [ sl_args.size() - sl_names.size() + i ];
            n_names << sl_names[ i ];
        }
        for( size_t i = 0; i < names.size(); ++i ) {
            n_args  << args [ args.size() - names.size() + i ];
            n_names << names[ i ];
        }
        return make_sl_trial( scope, want_ret, func, self, {}, {}, n_args, n_names, call_ctor );
    }

    Def *def = rcast( func.ptr() );

    //
    Bool_vec defined_args( def->arg_names.size(), false );

    // make a Sl_trial_Def
    Variable tr_var( scope->vm, scope->vm->type_SlTrialDef );
    SlTrialDef *tr = rcast( tr_var.ptr() );
    tr->args.resize( def->arg_names.size() );
    tr->def = func;

    // new scope, to compute condition, ...
    Scope new_scope( Scope::Scope_type::CALL, scope );
    if ( ! def->static_flag )
        new_scope.self = self;

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
                Rc_string err = constraint.valid_constraint_for( &new_scope, args[ n ].ugs( scope ), tr->tci );
                if ( err.size() )
                    return fail( "argument '" + names[ i ] + "' " + err );
            }
            defined_args.set( ind, true );
            new_scope.reg_var( def->arg_names[ ind ], args[ n ], Scope::VariableFlags::NONE, false );
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

    // self
    if ( Ast_crepr &sc = def->self_constraint ) {
        Variable constraint = new_scope.visit( sc, true ).ugs( scope );
        Rc_string err = constraint.valid_constraint_for( &new_scope, self.ugs( scope ), tr->tci );
        if ( err.size() )
            return fail( "self " + err );
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

Variable Type_Def::use_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &sl_var, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const {
    auto rs = raii_save( scope->vm->nb_calls );
    if ( ++scope->vm->nb_calls > 1000 ) {
        scope->add_error( "too much calls" );
        ERROR( "..." );
    }

    SurdefList *sd = rcast( sl_var.ptr() );
    SlTrialDef *st = rcast( trial.ptr() );
    Def *def = rcast( func.ptr() );

    // if method is virtual, we may have to call a different method instance
    if ( def->virtual_flag && ( call_ctor || def->name != "destroy" ) ) {
        if ( ! sd->self )
            scope->add_error( "call to a virtual def without object" );
        Vtable *vtable = *reinterpret_cast<Vtable **>( sd->self.ptr() );
        if ( vtable->type != sd->self.type ) {
            // make a new variable, based on the same content, with a different type
            Variable v( sd->self.content, sd->self.flags, vtable->type, sd->self.offB - vtable->offsetB );
            Variable m = v.find_attribute( scope, def->name );
            ASSERT( sl_args.empty() && sl_names.empty(), "" );
            return m.apply( scope, want_ret, args, names, call_ctor );
        }
    }

    Scope new_scope( Scope::Scope_type::CALL, scope );
    new_scope.self_method_name = def->name;
    new_scope.self = sd->self;

    for( const CatchedVariable &cv : def->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );
    for( size_t i = 0; i < def->arg_names.size(); ++i )
        new_scope.reg_var( def->arg_names[ i ], st->args[ i ] );
    for( const auto &v : st->tci.proposals )
        new_scope.reg_var( v.first, v.second );
    // new_scope.valid_scope_ptr = def->valid_scope_ptr.get_scope();
    new_scope.__arguments_names = def->arg_names;

    if ( def->name == "construct" && sd->self && sd->self.type->destroy_attrs() ) {
        new_scope.ctor_self = sd->self.ugs( scope );

        //        // default initializations
        //        new_scope.flags |= Scope::ScopeFlags::CONSTRUCTOR;
        //        RcPtr<Inst> tmp_room;
        //        if ( new_scope.caller && new_scope.caller->ctor_room_inst ) {
        //            new_scope.ctor_room_inst = new_scope.caller->ctor_room_inst;
        //            new_scope.ctor_self      = new_scope.caller->ctor_self;
        //        } else {
        //            tmp_room = new Room( new_scope.self.p_type( &new_scope ) );
        //            new_scope.ctor_room_inst = tmp_room.ptr();
        //            new_scope.ctor_self      = new_scope.self;
        //        }

        //        // set vtable pointers
        //        if ( call_ctor )
        //            new_scope.ctor_self.setup_vtables();

        // wpc
        std::set<Rc_string> wpc;
        new_scope.wpc = &wpc;
        new_scope.visit( def->wpc_code, false );

        // inheritance
        if ( Class *cl = new_scope.ctor_self.type->orig_class() ) {
            // catched variables
            for( const CatchedVariable &cv : cl->catched_variables )
                new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );

            //
            for( const std::string &inh_name : cl->inheritance_names ) {
                // alreay initialised by a init_of ?
                if ( wpc.count( inh_name ) )
                    continue;

                Variable supv = new_scope.ctor_self.find_attribute( &new_scope, inh_name, false );
                if ( ! supv )
                    return new_scope.add_error( "Impossible to find super '{}' in Value of type '{}'", inh_name, *new_scope.ctor_self.type ), scope->vm->ref_error;
                Variable ctor = supv.find_attribute( &new_scope, "construct" );
                ctor.apply( &new_scope, false, {}, {}, false );
            }
        } else {
            P( *new_scope.ctor_self.type );
            ERROR( "f" );
        }

        // put again the static variables
        for( auto p : sd->self.type->static_attributes )
            new_scope.reg_var( p.first, *p.second, Scope::VariableFlags::STATIC | Scope::VariableFlags::CATCHED );

        // set vtable pointers
        if ( call_ctor )
            new_scope.ctor_self.setup_vtables();

        // class block
        new_scope.visit( new_scope.ctor_self.type->orig_class()->block, false );

        // we have finished with preconstruction
        new_scope.ctor_self.clear();
        new_scope.futur_attrs.clear();
    }

    // return type
    Type *return_type = 0;
    if ( def->return_type )
        return_type = new_scope.visit( def->return_type, true ).apply( scope, true, {}, {}, false ).ugs_type( scope );


    // execution
    Variable ret = new_scope.visit( def->block, want_ret );
    if ( want_ret && new_scope.nb_breaks && new_scope.ret )
        ret = new_scope.ret;

    // return type
    if ( want_ret && return_type && ret.ugs_type( scope ) != return_type ) {
        Variable res = scope->vm->new_Type( return_type ).apply( &new_scope, true, ret );
        ret = res;
    }

    // post
    if ( def->name == "destroy" && sd->self && sd->self.type->destroy_attrs() ) {
        // we don't want sd->self to be destroyed again
        sd->self.content->cpt_use = 654;

        // destruction of attributes
        Variable s = sd->self.ugs( scope );
        for( Type::Attribute *attr = s.type->last_attribute; attr; attr = attr->prev ) {
            ASSERT( attr->off_in_bits % 8 == 0, "..." );
            Variable v( s.content, s.flags, attr->type, s.offB + attr->off_in_bits / 8 );
            v.type->destroy( scope, v, false );
        }
    }

    // if the converter is named, we return a null variable to signal the case
    if ( def->named_converter_flag )
        ret.clear();

    return ret;
}

