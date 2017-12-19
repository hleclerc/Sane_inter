#include "System/RaiiSave.h"
#include "System/BoolVec.h"
#include "SlTrialDef.h"
#include "SurdefList.h"
#include "Wildcard.h"
#include "TypeDef.h"
#include "Varargs.h"
#include "Class.h"
#include "Scope.h"
#include "Def.h"
#include "gvm.h"
#include <cmath>

TypeDef::TypeDef() : Type( "Def" ) {
}

Variable TypeDef::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) {
    TODO;
    return {};
}

double TypeDef::get_pertinence( const Variable &self ) const {
    Def *cl = self.rcast<Def>();
    return cl->def_pert_num * std::pow( 10.0, cl->def_pert_exp );
}

Variable TypeDef::make_sl_trial( bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) const {
    if ( sl_args.size() ) {
        Vec<Variable> n_args ( Rese(), sl_args .size() + args .size() );
        Vec<RcString> n_names( Rese(), sl_names.size() + names.size() );
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
        return make_sl_trial( want_ret, func, self, {}, {}, n_args, n_names, with_self, apply_flags );
    }

    Def *def = func.rcast<Def>();

    //
    BoolVec defined_args( def->arg_names.size(), false );

    // make a Sl_trial_Def
    Variable tr_var( new KnownRef<SlTrialDef>, gvm->type_SlTrialDef );
    SlTrialDef *tr = tr_var.rcast<SlTrialDef>();
    tr->args.resize( def->arg_names.size() );
    tr->def = func;

    // new scope, to compute condition, ...
    Scope new_scope( Scope::ScopeType::CALL );
    if ( ! def->static_flag )
        new_scope.self = self;

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
                RcString err = constraint.valid_constraint_for( args[ n ], tr->tci );
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

    // self
    if ( AstCrepr &sc = def->self_constraint ) {
        Variable constraint = gvm->visit( sc, true );
        RcString err = constraint.valid_constraint_for( self, tr->tci );
        if ( err.size() )
            return fail( "self " + err );
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

Variable TypeDef::use_sl_trial( bool want_ret, const Variable &func, const Variable &sl_var, const Vec<Variable> &sl_args, const Vec<RcString> &sl_names, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags, const Variable &trial ) const {
    auto rs = raii_save( gvm->nb_calls );
    if ( ++gvm->nb_calls > 1000 ) {
        gvm->add_error( "too much calls" );
        ERROR( "..." );
    }

    // SurdefList *sd = sl_var.rcast<SurdefList>();
    SlTrialDef *st = trial.rcast<SlTrialDef>();
    Def *def = func.rcast<Def>();

    // if method is virtual, we may have to call a different method instance
    if ( def->virtual_flag && ( ! ( apply_flags & ApplyFlags::DONT_CALL_CTOR ) || def->name != "destroy" ) ) {
        if ( ! with_self )
            gvm->add_error( "call to a virtual def without object" );
        TODO;
        //        Vtable *vtable = *reinterpret_cast<Vtable **>( with_self.ptr() );
        //        if ( vtable->type != with_self.type ) {
        //            // make a new variable, based on the same content, with a different type
        //            Variable v( with_self.content, with_self.flags, vtable->type, with_self.offB - vtable->offsetB );
        //            Variable m = v.find_attribute( def->name );
        //            ASSERT( sl_args.empty() && sl_names.empty(), "" );
        //            return m.apply( want_ret, args, names, call_ctor );
        //        }
    }

    Scope new_scope( Scope::ScopeType::CALL );
    new_scope.self_method_name = def->name;
    new_scope.self = with_self;

    for( const CatchedVariable &cv : def->catched_variables )
        new_scope.reg_var( cv.name, cv.val, Scope::VariableFlags::CATCHED );
    for( size_t i = 0; i < def->arg_names.size(); ++i )
        new_scope.reg_var( def->arg_names[ i ], st->args[ i ] );
    for( const auto &v : st->tci.proposals )
        new_scope.reg_var( v.first, v.second );
    // new_scope.valid_scope_ptr = def->valid_scope_ptr.get_scope();
    new_scope.__arguments_names = def->arg_names;

    if ( def->name == "construct" && with_self && with_self.type->destroy_attrs() ) {
        new_scope.ctor_self = with_self;

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
        std::set<RcString> wpc;
        new_scope.wpc = &wpc;
        gvm->visit( def->wpc_code, false );

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

                Variable supv = new_scope.ctor_self.find_attribute( inh_name, false );
                if ( ! supv )
                    return gvm->add_error( "Impossible to find super '{}' in Value of type '{}'", inh_name, *new_scope.ctor_self.type );
                Variable ctor = supv.find_attribute( "construct" );
                ctor.apply( false, {}, {}, ApplyFlags::DONT_CALL_CTOR );
            }
        } else {
            P( *new_scope.ctor_self.type );
            ERROR( "f" );
        }

        // put again the static variables
        for( auto p : with_self.type->content.data.static_attributes )
            new_scope.reg_var( p.first, *p.second, Scope::VariableFlags::STATIC | Scope::VariableFlags::CATCHED );

        // set vtable pointers
        if ( ! ( apply_flags & ApplyFlags::DONT_CALL_CTOR ) )
            new_scope.ctor_self.setup_vtables();

        // class block
        gvm->visit( new_scope.ctor_self.type->orig_class()->block, false );

        // we have finished with preconstruction
        new_scope.ctor_self.clear();
        new_scope.futur_attrs.clear();
    }

    // return type
    Type *return_type = 0;
    if ( def->return_type )
        return_type = gvm->visit( def->return_type, true ).apply( true, {}, {}, ApplyFlags::DONT_CALL_CTOR ).type;


    // execution
    Variable ret = gvm->visit( def->block, want_ret );
    if ( want_ret && gvm->nb_breaks && new_scope.ret )
        ret = new_scope.ret;

    // return type
    if ( want_ret && return_type && ret.type != return_type ) {
        Variable res = gvm->new_Type( return_type ).apply( true, ret );
        ret = res;
    }

    // post
    if ( def->name == "destroy" && with_self && with_self.type->destroy_attrs() ) {
        // we don't want with_self to be destroyed again
        with_self.ref->cpt_use = 654;

        // destruction of attributes
        for( TypeContent::Attribute *attr = with_self.type->content.data.last_attribute; attr; attr = attr->prev ) {
            ASSERT( attr->off % 8 == 0, "..." );
            Variable v( with_self.ref, attr->type, with_self.offset + attr->off, with_self.flags );
            v.type->destroy( v, false );
        }
    }

    // if the converter is named, we return a null variable to signal the case
    if ( def->named_converter_flag )
        ret.clear();

    return ret;
}

