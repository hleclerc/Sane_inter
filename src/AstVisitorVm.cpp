//#include "Ast_visitor_catched_variables.h"
#include "AstVisitorVm.h"
//#include "System/rcast.h"
//#include "System/LVec.h"
//#include "SurdefList.h"
//#include "GetSetter.h"
//#include "Lambda.h"
//#include "String.h"
//#include "Class.h"
//#include "Block.h"
//#include "Def.h"
//#include "AT.h"
#include "gvm.h"

//#include <experimental/filesystem>
//#include <Hpipe/TxtStream.h>

AstVisitorVm::AstVisitorVm( const RcString &names, bool want_ret ) : names( names ), want_ret( want_ret ) {
    rec = false;
}


void AstVisitorVm::set_src( size_t src, size_t off ) {
    gvm->pos.cur_names = names;
    gvm->pos.cur_src   = src;
    gvm->pos.cur_off   = off;
}

Variable AstVisitorVm::on_variable( RcString name ) {
    if ( Variable res = gvm->scope->find_variable( name, false ) )
        return res;
    return gvm->add_error( "Variable '{}' not found", name );
}

Variable AstVisitorVm::on_var_assembly( const Vec<RcString> &args ) {
    return on_variable( string_assembly( args ) );
}

Variable AstVisitorVm::on_break() {
    gvm->nb_breaks = gvm->scope->nb_scopes_to_break();
    return gvm->ref_void;
}

Variable AstVisitorVm::on_continue() {
    gvm->nb_breaks = gvm->scope->nb_scopes_to_cont();
    return gvm->ref_void;
}

Variable AstVisitorVm::on_self() {
    if ( Variable self = scope->find_self( false ) ) {
        ret_or_dec_ref( self );
    } else {
        scope->add_error( "'self' works only inside methods" );
        ret = vm->ref_error;
    }
}

Variable AstVisitorVm::on_this() {
    if ( Variable self = scope->find_self( false ) ) {
        Variable s = self.ugs( scope );

        Variable at_var( vm, vm->type_AT );
        AT *at = rcast( at_var.ptr() );
        at->ptr->content = s.content.ptr();
        at->ptr->offset_in_bytes = s.offB;

        ret_or_dec_ref( scope->find_variable( "Ptr" ).select( scope, true, vm->new_Type( s.type ) ).apply( scope, want_ret, at_var ) );
    } else {
        scope->add_error( "'this' works only inside methods" );
        ret = vm->ref_error;
    }
}

Variable AstVisitorVm::on_pass() {
    ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_false() {
    ret_or_dec_ref( vm->new_Bool( false ) );
}

Variable AstVisitorVm::on_true() {
    ret_or_dec_ref( vm->new_Bool( true ) );
}

Variable AstVisitorVm::on_number( RcString value ) {
    Hpipe::TxtStream<RcString> ts( &value );
    ret_or_dec_ref( vm->new_SI32( ts.read_unsigned() ) );
}

Variable AstVisitorVm::on_string( RcString value ) {
    Variable res( scope->vm, scope->vm->type_String );
    reinterpret_cast<String *>( res.ptr() )->init( value );
    ret_or_dec_ref( res );
}

Variable AstVisitorVm::on_get_attr( RcString obj, RcString name ) {
    Variable obj_ref = scope->visit( names, obj, true );
    if ( obj_ref.error() ) {
        if ( want_ret )
            ret = vm->ref_error;
        return;
    }

    if ( Variable res = obj_ref.find_attribute( scope, name, false ) ) {
        if ( want_ret )
            ret = res;
    } else {
        scope->add_error( "class '{}' has no member named '{}'", *obj_ref.ugs_type( scope ), name );
        if ( want_ret )
            ret = vm->ref_error;
    }
}

Variable AstVisitorVm::on_get_attr_ptr( RcString obj, RcString name ) {
    Variable obj_ptr = scope->visit( names, obj, true );
    Variable get_ref = obj_ptr.find_attribute( scope, "operator @", false );
    if ( ! get_ref )
        return scope->add_error( "class '{}' has no 'operator@' method", *obj_ptr.ugs_type( scope ), name ), ret_or_dec_ref( vm->ref_error );
    Variable obj_ref = get_ref.apply( scope, true );
    Variable res = obj_ref.find_attribute( scope, name, false );
    if ( ! res && ! obj_ref.error() )
        return scope->add_error( "class '{}' has no member named '{}'", *obj_ref.ugs_type( scope ), name ), ret_or_dec_ref( vm->ref_error );
    ret_or_dec_ref( res );
}

Variable AstVisitorVm::on_scope_resolution( RcString obj, RcString name ) {
    Variable obj_type = scope->visit( names, obj, true );
    Variable obj_inst = obj_type.apply( scope, true, {}, {}, false );
    Type *type = obj_inst.ugs_type( scope );
    if ( type == vm->type_Error )
        return ret_or_dec_ref( vm->ref_error );

    //Variable res = type->find_attribute( scope, name, 0, 0, 0, Variable::Flags::NONE, 0 );
    Variable res = obj_inst.find_attribute( scope, name, false );
    if ( ! res )
        return scope->add_error( "class '{}' has no static member named '{}'", *type, name ), ret_or_dec_ref( vm->ref_error );
    ret_or_dec_ref( res );
}

Variable AstVisitorVm::on_get_attr_calc( RcString obj, const Vec<RcString> &args ) {
    on_get_attr( obj, string_assembly( args ) );
}

Variable AstVisitorVm::on_get_attr_ptr_calc( RcString obj, const Vec<RcString> &args ) {
    on_get_attr_ptr( obj, string_assembly( args ) );
}

Variable AstVisitorVm::on_scope_resolution_calc( RcString obj, const Vec<RcString> &args ) {
    on_scope_resolution( obj, string_assembly( args ) );
}

Variable AstVisitorVm::on_block( const Vec<RcString> &items ) {
    Scope ns( Scope::Scope_type::BLOCK, scope );
    if ( items.size() ) {
        for( size_t i = 0; i < items.size() - 1; ++i )
            ns.visit( names, items[ i ], false );
        ret_or_dec_ref( ns.visit( names, items.back(), want_ret ) );
    } else
        ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_apply_op( RcString f, const Vec<RcString> &args ) {
    // make arguments
    Vec<Variable> v_args( Rese(), args.size() );
    for( const RcString &arg : args )
        v_args << scope->visit( names, arg, true );

    for( const Variable &v : v_args )
        if ( v.error() )
            return ret_or_dec_ref( vm->ref_error );

    // find lhs
    Variable func = v_args[ 0 ].find_attribute( scope, f, false );
    if ( ! func )
        return scope->add_error( "Operator '{}' is not defined (for type {})", f, *v_args[ 0 ].type ), ret_or_dec_ref( vm->ref_error );
    ret_or_dec_ref( func.apply( scope, want_ret, v_args.from( 1 ) ) );
}

Variable AstVisitorVm::on_apply( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) {
    Scope::Position pos = scope->pos;

    // make lhs + arguments
    Variable func = scope->visit( this->names, f, true );
    Vec<RcString> v_names( Rese(), names.size() );
    Vec<Variable> v_args( Rese(), args.size() );
    bool has_err = false;
    for( size_t i = 0, n = args.size() - names.size(); i < args.size(); ++i ) {
        Variable ref = scope->visit( this->names, args[ i ], true );
        if ( spreads.contains( i ) ) {
            if ( ref.error() )
                has_err = true;
            else
                ref.type->spread_in( scope, ref, v_args, v_names );
        } else {
            if ( i < n )
                v_args.insert( v_args.size() - v_names.size(), ref );
            else {
                v_names << names[ i - n ];
                v_args << ref;
            }
        }
    }
    // launch
    scope->pos = pos;
    ret = has_err ? vm->ref_error : func.apply( scope, want_ret, v_args, v_names );
}

Variable AstVisitorVm::on_select( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) {
    // make lhs + arguments
    Variable func = scope->visit( this->names, f, true );
    Vec<RcString> v_names( Rese(), names.size() );
    Vec<Variable> v_args( Rese(), args.size() );
    bool has_err = false;
    for( size_t i = 0, n = args.size() - names.size(); i < args.size(); ++i ) {
        Variable ref = scope->visit( this->names, args[ i ], true );
        if ( spreads.contains( i ) ) {
            if ( ref.error() )
                has_err = true;
            else
                ref.type->spread_in( scope, ref, v_args, v_names );
        } else {
            if ( i < n )
                v_args.insert( v_args.size() - v_names.size(), ref );
            else {
                v_names << names[ i - n ];
                v_args << ref;
            }
        }
    }
    // launch
    ret = has_err ? vm->ref_error : func.select( scope, want_ret, v_args, names );
}

Variable AstVisitorVm::on_chbeba( RcString f, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) {
    // make lhs + arguments
    Variable func = scope->visit( this->names, f, true );
    Vec<RcString> v_names( Rese(), names.size() );
    Vec<Variable> v_args( Rese(), args.size() );
    bool has_err = false;
    for( size_t i = 0, n = args.size() - names.size(); i < args.size(); ++i ) {
        Variable ref = scope->visit( this->names, args[ i ], true );
        if ( spreads.contains( i ) ) {
            if ( ref.error() )
                has_err = true;
            else
                ref.type->spread_in( scope, ref, v_args, v_names );
        } else {
            if ( i < n )
                v_args.insert( v_args.size() - v_names.size(), ref );
            else {
                v_names << names[ i - n ];
                v_args << ref;
            }
        }
    }
    // launch
    ret = has_err ? vm->ref_error : func.chbeba( scope, want_ret, v_args, names );
}


Variable AstVisitorVm::on_init_of( RcString name, const Vec<RcString> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) {
    // get arg values
    Vec<Variable> v_args( Rese(), args.size() );
    for( const RcString &arg : args )
        v_args << scope->visit( this->names, arg, true );

    // call the helper
    init_of( scope, name, v_args, names, spreads );


    ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_assign( RcString name, RcString cname, PI8 nb_scopes_rec, RcString value, PI8 flags ) {
    Scope *store = scope;
    while ( nb_scopes_rec-- ) {
        store = store->parent_interp( true );
        if ( ! store )
            return scope->add_error( "Not enough scope parents" ), ret_or_dec_ref( vm->ref_error );
    }

    if ( name.empty() ) {
        ASSERT( cname.size(), "..." );
        Variable vn = scope->find_variable( "String" ).apply( scope, true, scope->visit( this->names, cname, true ) );
        name = *reinterpret_cast<String *>( vn.ptr() );
    }

    ret_or_dec_ref( assign( store, name, [&]() { return scope->visit( names, value, true ); }, flags ) );
}

Variable AstVisitorVm::on_ss_block( const Vec<RcString> &items ) {
    ret_or_dec_ref( scope->visit( names, items, want_ret ) );
}

Variable AstVisitorVm::on_destruct_assign( const Vec<RcString> &names, const Vec<RcString> &def_vals, const Vec<RcString> &types, RcString value, PI8 flags ) {
    Variable rhs = scope->visit( this->names, value, true );

    // for each name
    for( size_t i = 0; i < names.size(); ++i ) {
        Variable vhs = rhs.find_attribute( scope, names[ i ], false );
        if ( ! vhs ) {
            if ( def_vals[ i ] ) {
                vhs = scope->visit( this->names, def_vals[ i ], true );
            } else {
                scope->add_error( "There's no attribute '{}' in rhs", names[ i ] );
                vhs = vm->ref_error;
            }
        }
        if ( types[ i ] )
            TODO;

        assign( scope, names[ i ], [&]() { return vhs; }, flags );
    }

    ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_def( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, RcString self_constraint, RcString return_type, const Vec<RcString> &wpc ) {
    Scope *store = scope;
    while ( nb_scopes_rec-- ) {
        store = store->parent_interp( true );
        if ( ! store )
            return scope->add_error( "Not enough scope parents" ), ret_or_dec_ref( vm->ref_error );
    }

    if ( store->ctor_self )
        return;

    // get name
    if ( name.empty() ) {
        ASSERT( cname.size(), "..." );
        Variable vn = scope->find_variable( "String" ).apply( scope, true, scope->visit( this->names, cname, true ) );
        if ( vn.error() )
            return scope->add_error( "Not enough scope parents" ), ret_or_dec_ref( vm->ref_error );
        name = *reinterpret_cast<String *>( vn.ptr() );
    }

    // helper
    std::function<Ast_crepr(const RcString &)> ms = [&]( const RcString &cm ) { return Ast_crepr{ this->names, cm }; };

    // enrich or make a surdef (we have to make this before the class creation, to get the class as catched variable if necessary)
    Scope *l_scope = flags & CALLABLE_FLAG_global ? scope->root : store;
    auto iter_scope = l_scope->variables.find( name );
    Variable sl_var;
    if ( iter_scope != l_scope->variables.end() ) {
        sl_var = iter_scope->second.var;
        if ( sl_var.type != vm->type_SurdefList )
            return l_scope->add_error( "Variable '{}' already exists and it's not a surdef (=> impossible to register a class with this name)" ), ret_or_dec_ref( vm->ref_error );
        if ( flags & CALLABLE_FLAG_export )
            iter_scope->second.flags |= Scope::VariableFlags::SELF_AS_ARG;
    } else {
        Scope::VariableFlags vf = Scope::VariableFlags::CALLABLE;
        if ( flags & CALLABLE_FLAG_self_as_arg ) vf |= Scope::VariableFlags::SELF_AS_ARG;
        if ( flags & CALLABLE_FLAG_global      ) vf |= Scope::VariableFlags::GLOBAL;
        if ( flags & CALLABLE_FLAG_export      ) vf |= Scope::VariableFlags::EXPORT;
        if ( flags & CALLABLE_FLAG_static      ) vf |= Scope::VariableFlags::STATIC;

        //
        sl_var = Variable( vm, vm->type_SurdefList );
        l_scope->reg_var( name, sl_var, vf );
    }
    SurdefList *sl = rcast( sl_var.ptr() );

    // store a new Reference in the surdef list
    Variable var_def( vm, vm->type_Def );
    sl->lst << var_def;

    Def *def = rcast( var_def.ptr() );
    def->name                 = name;
    def->arg_names            = arg_names;
    def->arg_constraints      = arg_constraints.map( ms );
    def->arg_def_vals         = arg_def_vals.map( ms );
    def->arg_spreads          = arg_spreads;
    def->with_names           = with_names;
    def->with_constraints     = with_constraints.map( ms );
    def->with_def_vals        = with_def_vals.map( ms );
    def->with_spreads         = with_spreads;
    def->condition            = ms( condition  );
    def->pert_code            = ms( pertinence );
    def->def_pert_num         = def_pert_num;
    def->def_pert_exp         = def_pert_exp;
    def->block                = block.map( ms );
    def->self_constraint      = ms( self_constraint );
    def->return_type          = ms( return_type );
    def->wpc_code             = wpc.map( ms );
    def->source               = scope->src_name( scope->pos.cur_src );
    def->offset               = scope->pos.cur_off;
    def->self_as_arg_flag     = flags & CALLABLE_FLAG_self_as_arg;
    def->static_flag          = flags & CALLABLE_FLAG_static;
    def->abstract_flag        = flags & CALLABLE_FLAG_abstract;
    def->virtual_flag         = flags & ( CALLABLE_FLAG_abstract | CALLABLE_FLAG_virtual | CALLABLE_FLAG_override );
    def->override_flag        = flags & CALLABLE_FLAG_override;
    def->inline_flag          = flags & CALLABLE_FLAG_inline;
    def->named_converter_flag = flags & CALLABLE_FLAG_named_converter;
    // def->valid_scope_ptr.set_scope( scope );

    //
    auto reg_cv = [&]( const Ast_visitor_catched_variables &avcv, Vec<CatchedVariable> &vcv ) {
        for( RcString catched : avcv.catched )
            if ( Variable ref = scope->find_variable( catched, false, false, true ) )
                vcv << CatchedVariable{ catched, ref };
    };


    // get catched variables for the preparation
    Ast_visitor_catched_variables avcv_prep;
    for( const RcString &name : arg_names        ) avcv_prep.internal.insert( name );
    for( const RcString &name : with_names       ) avcv_prep.internal.insert( name );
    for( const RcString &code : arg_constraints  ) ast_visit( avcv_prep, code );
    for( const RcString &code : arg_def_vals     ) ast_visit( avcv_prep, code );
    for( const RcString &code : with_constraints ) ast_visit( avcv_prep, code );
    for( const RcString &code : with_def_vals    ) ast_visit( avcv_prep, code );
    ast_visit( avcv_prep, condition  );
    ast_visit( avcv_prep, pertinence );
    reg_cv( avcv_prep, def->catched_variables_prep );

    // get catched variables for the block
    Ast_visitor_catched_variables avcv;
    for( const RcString &cm   : block      ) avcv.pre_visit( cm );
    for( const RcString &name : arg_names  ) avcv.internal.insert( name );
    for( const RcString &name : with_names ) avcv.internal.insert( name );
    for( const RcString &code : wpc        ) ast_visit( avcv, code );
    for( const RcString &code : block      ) ast_visit( avcv, code );
    ast_visit( avcv, return_type );
    reg_cv( avcv, def->catched_variables );


    // return
    ret_or_dec_ref( sl_var );
}

Variable AstVisitorVm::on_class( RcString name, RcString cname, PI8 nb_scopes_rec, const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_spreads, RcString condition, RcString pertinence, SI32 def_pert_num, SI32 def_pert_exp, PI32 flags, const Vec<RcString> &block, const Vec<RcString> &inheritance_names, const Vec<RcString> &inheritance ) {
    Scope *store = scope;
    while ( nb_scopes_rec-- ) {
        store = store->parent_interp( true );
        if ( ! store )
            return scope->add_error( "Not enough scope parents" ), ret_or_dec_ref( vm->ref_error );
    }

    if ( store->ctor_self )
        return;

    // get name
    if ( name.empty() ) {
        ASSERT( cname.size(), "..." );
        Variable vn = scope->find_variable( "String" ).apply( scope, true, scope->visit( this->names, cname, true ) );
        if ( vn.error() )
            return scope->add_error( "Not enough scope parents" ), ret_or_dec_ref( vm->ref_error );
        name = *reinterpret_cast<String *>( vn.ptr() );
    }

    // helper
    std::function<Ast_crepr(const RcString &)> ms = [&]( const RcString &cm ) { return Ast_crepr{ this->names, cm }; };

    // enrich or make a surdef (we have to make this before the class creation, to get the class as catched variable if necessary)
    Scope *l_scope = flags & CALLABLE_FLAG_global ? scope->root : store;
    auto iter_scope = l_scope->variables.find( name );
    Variable sl_var;
    if ( iter_scope != l_scope->variables.end() ) {
        sl_var = iter_scope->second.var;
        if ( sl_var.type != vm->type_SurdefList )
            return l_scope->add_error( "Variable '{}' already exists and it's not a surdef (=> impossible to register a class with this name)" ), ret_or_dec_ref( vm->ref_error );
        if ( flags & CALLABLE_FLAG_export )
            iter_scope->second.flags |= Scope::VariableFlags::SELF_AS_ARG;
    } else {
        Scope::VariableFlags vf = Scope::VariableFlags::CALLABLE | Scope::VariableFlags::STATIC;
        if ( flags & CALLABLE_FLAG_self_as_arg ) vf |= Scope::VariableFlags::SELF_AS_ARG;
        if ( flags & CALLABLE_FLAG_global      ) vf |= Scope::VariableFlags::GLOBAL;
        if ( flags & CALLABLE_FLAG_export      ) vf |= Scope::VariableFlags::EXPORT;

        //
        sl_var = Variable( vm, vm->type_SurdefList );
        l_scope->reg_var( name, sl_var, vf );
    }
    SurdefList *sl = reinterpret_cast<SurdefList *>( sl_var.ptr() );

    // store a new Reference in the surdef list
    Variable var_def( vm, vm->type_Class );
    vm->main_scope.add_static_variable( var_def );
    sl->lst << var_def;

    Class *def = rcast( var_def.ptr() );
    def->name              = name;
    def->arg_names         = arg_names;
    def->arg_constraints   = arg_constraints.map( ms );
    def->arg_def_vals      = arg_def_vals   .map( ms );
    def->arg_spreads       = arg_spreads;
    def->with_names        = with_names;
    def->with_constraints  = with_constraints.map( ms );
    def->with_def_vals     = with_def_vals.map( ms );
    def->with_spreads      = with_spreads;
    def->condition         = ms( condition  );
    def->pert_code         = ms( pertinence );
    def->def_pert_num      = def_pert_num;
    def->def_pert_exp      = def_pert_exp;
    def->block             = block.map( ms );
    def->source            = scope->src_name( scope->pos.cur_src );
    def->offset            = scope->pos.cur_off;
    def->inheritance       = inheritance.map( ms );
    def->inheritance_names = inheritance_names;
    def->export_flag       = flags & CALLABLE_FLAG_export;
    def->global_flag       = flags & CALLABLE_FLAG_global;
    // def->valid_scope_ptr.set_scope( scope );

    //
    auto reg_cv = [&]( const Ast_visitor_catched_variables &avcv, Vec<CatchedVariable> &vcv ) {
        for( RcString catched : avcv.catched )
            if ( Variable ref = scope->find_variable( catched, false, false ) )
                vcv << CatchedVariable{ catched, ref };
    };

    // get catched variables for the condition
    Ast_visitor_catched_variables avcv_prep;
    for( const RcString &name : arg_names        ) avcv_prep.internal.insert( name );
    for( const RcString &name : with_names       ) avcv_prep.internal.insert( name );
    for( const RcString &code : inheritance      ) ast_visit( avcv_prep, code );
    for( const RcString &code : arg_constraints  ) ast_visit( avcv_prep, code );
    for( const RcString &code : arg_def_vals     ) ast_visit( avcv_prep, code );
    for( const RcString &code : with_constraints ) ast_visit( avcv_prep, code );
    for( const RcString &code : with_def_vals    ) ast_visit( avcv_prep, code );
    ast_visit( avcv_prep, condition  );
    ast_visit( avcv_prep, pertinence );
    reg_cv( avcv_prep, def->catched_variables_prep );

    // get catched variables for the inheritance
    def->catched_variables_inh.resize( inheritance.size() );
    for( size_t i = 0; i < inheritance.size(); ++i ) {
        Ast_visitor_catched_variables avcv_inh;
        ast_visit( avcv_inh, inheritance[ i ] );
        reg_cv( avcv_inh, def->catched_variables_inh[ i ] );
    }

    // get catched variables for the block
    Ast_visitor_catched_variables avcv;
    for( const RcString &cm : block           ) avcv.pre_visit( cm );
    for( const RcString &name : arg_names     ) avcv.internal.insert( name );
    for( const RcString &name : with_names    ) avcv.internal.insert( name );
    for( const RcString &cm : arg_constraints ) ast_visit( avcv, cm );
    for( const RcString &cm : arg_def_vals    ) ast_visit( avcv, cm );
    for( const RcString &cm : block           ) ast_visit( avcv, cm );
    for( const RcString &cm : inheritance     ) ast_visit( avcv, cm );
    ast_visit( avcv, pertinence );
    reg_cv( avcv, def->catched_variables );

    // if it's a base class, make a tmp instance to complete the type information
    if ( vm->init_mode && vm->base_types.count( name ) )
        sl_var.apply( scope, false, {}, {}, false );

    // return
    ret_or_dec_ref( sl_var );
}

Variable AstVisitorVm::on_if( RcString cond, const Vec<RcString> &ok ) {
    on_if_else( cond, ok, {} );
}

Variable AstVisitorVm::on_if_else( RcString cond, const Vec<RcString> &ok, const Vec<RcString> &ko ) {
    Scope cond_scope( Scope::Scope_type::BLOCK, scope );

    Variable cond_var = cond_scope.visit( names, cond, true ).ugs( scope );
    if ( cond_var.type != cond_scope.vm->type_Bool ) {
        Variable res = cond_scope.find_variable( "Bool" ).apply( &cond_scope, true, cond_var ).ugs( &cond_scope );
        cond_var = res;
        if ( cond_var.type != cond_scope.vm->type_Bool ) {
            if ( ! cond_var.error() )
                cond_scope.add_error( "conv to Bool should give a Bool" );
            return ret_or_dec_ref( vm->ref_error );
        }
    }

    Scope new_scope( Scope::Scope_type::IF_EXE, &cond_scope );
//    static int cpt = 0;
//    if( ++cpt == 16 )
//        scope->add_error("pouet");
//    P( cond_var );
    ret_or_dec_ref( new_scope.visit( names, cond_var.is_true( &new_scope ) ? ok : ko, want_ret ) );
}

Variable AstVisitorVm::on_while( RcString cond, const Vec<RcString> &ok ) {
    size_t os = vm->el.size();
    while ( true ) {
        Scope new_scope( Scope::Scope_type::WHILE, scope );
        Variable cond_var = new_scope.visit( names, cond, true );
        if ( ! cond_var.is_true( scope ) )
            break;
        new_scope.visit( names, ok, false );
        if ( os != vm->el.size() || new_scope.nb_breaks || new_scope.nb_conts > 1 )
            break;
    }
    ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_return( RcString value ) {
    ret = scope->visit( names, value, true );
    scope->nb_breaks = scope->nb_scopes_to_ret();
    scope->ret = ret;
}

Variable AstVisitorVm::on_for( RcString name, RcString container, const Vec<RcString> &code ) {
    // value container
    Variable value = scope->visit( names, container, true );
    if ( value.error() )
        return ret_or_dec_ref( vm->ref_error );

    //
    std::function<Ast_crepr(const RcString &)> ms = [&]( const RcString &cm ) { return Ast_crepr{ names, cm }; };

    // make a new Reference for the Block
    Variable block_var( vm, vm->type_Block );
    Block *block = rcast( block_var.ptr() );
    // block->valid_scope_ptr.set_scope( scope );
    block->code = code.map( ms );
    block->name = name;

    // get catched variables
    Ast_visitor_catched_variables avcv;
    avcv.internal.insert( name );
    for( const RcString &cm : code )
        ast_visit( avcv, cm );

    // register them (if found)
    for( RcString catched : avcv.catched )
        if ( Variable ref = scope->find_variable( catched, false, false ) )
            block->catched_variables << CatchedVariable{ catched, ref };

    //
    Scope new_scope( Scope::Scope_type::FOR_BEG, scope );

    // call operator for with the block
    Variable operator_for = value.find_attribute( &new_scope, "operator for", false );
    if ( ! operator_for )
        return scope->add_error( "Impossible to find an 'operator for' method for object of type '" + to_string( *value.ugs_type( scope ) ) + "'" );
    ret_or_dec_ref( operator_for.apply( &new_scope, want_ret, block_var ) );
}

Variable AstVisitorVm::on_typeof( RcString value ) {
    if ( want_ret )
        ret = xxxxof( value, 0 );
}

Variable AstVisitorVm::on_sizeof( RcString value ) {
    if ( want_ret )
        ret = xxxxof( value, 1, true );
}

Variable AstVisitorVm::on_aligof( RcString value ) {
    if ( want_ret )
        ret = xxxxof( value, 2, true );
}

Variable AstVisitorVm::on_sizeof_bits( RcString value ) {
    if ( want_ret )
        ret = xxxxof( value, 1 );
}

Variable AstVisitorVm::on_aligof_bits( RcString value ) {
    if ( want_ret )
        ret = xxxxof( value, 2 );
}

Variable AstVisitorVm::on_lambda( const Vec<RcString> &arg_names, const Vec<RcString> &arg_constraints, const Vec<RcString> &arg_def_vals, const Vec<size_t> &arg_spreads, RcString body ) {
    std::function<Ast_crepr(const RcString &)> ms = [&]( const RcString &cm ) { return Ast_crepr{ this->names, cm }; };

    Variable var_lambda( vm, vm->type_Lambda );
    Lambda *lambda = rcast( var_lambda.ptr() );

    lambda->arg_names       = arg_names;
    lambda->arg_constraints = arg_constraints.map( ms );
    lambda->arg_def_vals    = arg_def_vals.map( ms );
    lambda->arg_spreads     = arg_spreads;
    lambda->body            = ms( body );
    lambda->source          = scope->src_name( scope->pos.cur_src );
    lambda->offset          = scope->pos.cur_off;

    //
    auto reg_cv = [&]( const Ast_visitor_catched_variables &avcv, Vec<CatchedVariable> &vcv ) {
        for( RcString catched : avcv.catched )
            if ( Variable ref = scope->find_variable( catched, false, false, arg_names.size() && arg_names[ 0 ] == "self" ) )
                vcv << CatchedVariable{ catched, ref };
    };

    // get catched variables for the block
    Ast_visitor_catched_variables avcv;
    for( const RcString &name : arg_names     ) avcv.internal.insert( name );
    for( const RcString &cm : arg_constraints ) ast_visit( avcv, cm );
    for( const RcString &cm : arg_def_vals    ) ast_visit( avcv, cm );
    ast_visit( avcv, body );
    reg_cv( avcv, lambda->catched_variables );

    // return
    ret_or_dec_ref( var_lambda );
}

Variable AstVisitorVm::on_and( RcString v0, RcString v1 ) {
    Variable va0 = scope->visit( names, v0, true ), b0 = va0.ugs( scope );

    if ( b0.type != scope->vm->type_Bool ) {
        Variable res = scope->find_variable( "Bool" ).apply( scope, true, b0 ).ugs( scope );
        b0 = res;
        if ( b0.type != scope->vm->type_Bool ) {
            if ( ! b0.error() )
                scope->add_error( "conv to Bool should give a Bool" );
            return ret_or_dec_ref( vm->ref_error );
        }
    }

    ret_or_dec_ref( b0.is_false( scope ) ? va0 : scope->visit( names, v1, true ) );
}

Variable AstVisitorVm::on_or( RcString v0, RcString v1 ) {
    Variable va0 = scope->visit( names, v0, true ), b0 = va0.ugs( scope );

    if ( b0.type != scope->vm->type_Bool ) {
        Variable res = scope->find_variable( "Bool" ).apply( scope, true, b0 ).ugs( scope );
        b0 = res;
        if ( b0.type != scope->vm->type_Bool ) {
            if ( ! b0.error() )
                scope->add_error( "conv to Bool should give a Bool" );
            return ret_or_dec_ref( vm->ref_error );
        }
    }

    ret_or_dec_ref( b0.is_true( scope ) ? va0 : scope->visit( names, v1, true ) );
}

Variable AstVisitorVm::on_vec( const Vec<RcString> &values ) {
    Variable res( vm, vm->type_CtVec );
    LVec<Variable> *vec = rcast( res.ptr() );
    vec->reserve( values.size() );
    for( const RcString &code : values )
        *vec << scope->visit( names, code, true );
    ret_or_dec_ref( res );
}

Variable AstVisitorVm::on_map( const Vec<RcString> &keys, const Vec<RcString> &vals ) {
    Variable res( vm, vm->type_CtMap );
    std::pair<LVec<Variable>,LVec<Variable>> *vec = rcast( res.ptr() );
    vec->first .reserve( keys.size() );
    vec->second.reserve( vals.size() );
    for( const RcString &code : keys )
        vec->first  << scope->visit( names, code, true );
    for( const RcString &code : vals )
        vec->second << scope->visit( names, code, true );
    ret_or_dec_ref( res );
}

Variable AstVisitorVm::on_matrix( const Vec<RcString> &values, const Vec<size_t> &sizes ) {
    TODO;
}

Variable AstVisitorVm::on_raii( const Vec<RcString> &code ) {
    for( const RcString &str : code )
        assign( scope, "", [&]() { return scope->visit( names, str, true ); }, 0 );
}

Variable AstVisitorVm::on_info( const Vec<RcString> &str, const Vec<RcString> &code ) {
    for( size_t i = 0; i < str.size(); ++i ) {
        Variable value = scope->visit( names, code[ i ], true );
        *vm->os << str[ i ] << " => " << std::flush;
        if ( Variable ws = value.find_attribute( scope, "write_to_stream", false ) )
            ws.apply( scope, false, scope->find_variable( "stderr" ) );
        else
            scope->add_error( "object {} has no write_to_stream attribute", *value.ugs_type( scope ) );
        if ( i + 1 == str.size() )
            *vm->os << std::endl;
        else
            *vm->os << ", " << std::flush;
    }
    ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_assert( RcString str, RcString code ) {
    Variable cond_var = scope->visit( names, code, true ).ugs( scope );
    if ( cond_var.type != scope->vm->type_Bool ) {
        Variable res = scope->find_variable( "Bool" ).apply( scope, true, cond_var ).ugs( scope );
        cond_var = res;
        if ( cond_var.type != scope->vm->type_Bool ) {
            if ( ! cond_var.error() )
                scope->add_error( "conv to Bool should give a Bool" );
            return ret_or_dec_ref( vm->ref_error );
        }
    }

    if ( cond_var.is_false( scope ) ) {
        scope->add_error( "assertion '{}' not checked", str );
        throw scope->exception( scope->find_variable( "AssertionError" ).apply( scope, true ), true );
    }
}

Variable AstVisitorVm::on_try_catch( RcString try_body, RcString arg_name, RcString arg_constraint, RcString catch_body, const Vec<RcString> &with_names, const Vec<RcString> &with_constraints, const Vec<RcString> &with_def_vals, const Vec<size_t> &with_splats ) {
    try {
        Scope new_scope( Scope::Scope_type::TRY, scope );
        ret_or_dec_ref( new_scope.visit( names, try_body, want_ret ) );
    } catch ( const Scope::Exception &e ) {
        Scope new_scope( Scope::Scope_type::CATCH, scope );
        new_scope.reg_var( arg_name, e.var );
        ret_or_dec_ref( new_scope.visit( names, catch_body, want_ret ) );
    }
}

Variable AstVisitorVm::on_import( RcString filename, const Vec<RcString> &names, const Vec<RcString> &args ) {
    std::experimental::filesystem::path orig( scope->src_name( scope->pos.cur_src ) );
    Variable mod = vm->import( filename + ".met", orig.parent_path().string() );

    // for each name
    for( size_t i = 0; i < names.size(); ++i ) {
        Variable vhs = mod.find_attribute( scope, args[ i ], false );
        if ( ! vhs ) {
            scope->add_error( "There's no attribute '{}' in rhs", names[ i ] );
            vhs = vm->ref_error;
        }

        assign( scope, names[ i ], [&]() { return vhs; }, ASSIGN_FLAG_ref | ASSIGN_FLAG_static );
    }

    ret_or_dec_ref( vm->ref_void );
}

Variable AstVisitorVm::on_enum( RcString name, const Vec<RcString> &items ) {
    //    // new Class
    //    Variable *v_class = vm->main_scope.add_static_variable( { vm, vm->type_Class } );
    //    Class *c = rcast( v_class->ptr() );
    //    c->inheritance_names << "super";
    //    c->source = scope->src_name( scope->pos.cur_src );
    //    c->offset = scope->pos.cur_off;
    //    c->name   = name;

    //    // new Type
    //    Type *type = vm->type_ptr_for( name, {} );
    //    type->orig = v_class;
    //    type->add_attribute( "super", 0, vm->type_SI32 );

    //    int cpt = 0;
    //    for( RcString &item : items ) {
    //        if ( type->static_attributes.count( item ) )
    //            scope->add_error( "name {} appears twice", item );
    //        type->static_attributes[ item ] = vm->main_scope.add_static_variable( vm->new_SI32( cpt++ ) );
    //    }

    //    Variable res = vm->new_Type( type );
    //    scope->reg_var( name, res, Scope::VariableFlags::STATIC );
    //    ret_or_dec_ref( res );
    if ( scope->ctor_self ) {
        ASSERT( want_ret == false, "..." );
        return ret_or_dec_ref( scope->vm->ref_void );
    }

    Variable lst_names = scope->find_variable( "Vec" ).select( scope, true, vm->new_Type( vm->type_String ) ).apply( scope, true );
    for( const RcString &item : items )
        lst_names.find_attribute( scope, "push_back" ).apply( scope, false, vm->new_String( item ) );
    Variable res = scope->find_variable( "Enum" ).select( scope, true, { vm->new_SI32( vm->nb_enum++ ), vm->new_String( name ), lst_names } );
    scope->reg_var( name, res, Scope::VariableFlags::STATIC );
    ret_or_dec_ref( res );
}

Variable AstVisitorVm::default_enter( const char *name ) {
    scope->add_error( "TODO: {}", name );
}

Variable AstVisitorVm::init_of( Scope *scope, RcString name, const Vec<Variable> &args, const Vec<RcString> &names, const Vec<size_t> &spreads ) {
    // find the scope used for class construction
    for( Scope *s = scope; s; s = s->parent ) {
        if ( s->wpc ) {
            // if we have a wpc, we have a self
            Variable self = s->self.ugs( scope );

            // special case (init_of self, ...)
            if ( name == "self" ) {
                // call the other constructor
                self.find_attribute( scope, "construct" ).apply( scope, false, args, names, false, spreads );

                // say that we have initialized everything
                for( auto p : self.type->attributes  )
                    s->wpc->insert( p.second.name );

                return;
            }

            // say that attribute $name is already initialized
            if ( s->wpc->count( name ) )
                return scope->add_error( "attribute {} is already initialised", name );
            s->wpc->insert( name );

            // instance attribute
            auto iter_attr = self.type->attributes.find( name );
            if ( iter_attr == self.type->attributes.end() )
                return scope->add_error( "there's no attribute {} in a {}", name, *self.type );
            Variable attr( self.content, self.flags, iter_attr->second.type, self.offB + iter_attr->second.off_in_bits / 8 );
            attr.find_attribute( scope, "construct" ).apply( scope, false, args, names, false, spreads );

            return;
        }
    }
    scope->add_error( "'init_of' instruction must be defined inside 'wpc' sequences" );

}

Variable AstVisitorVm::xxxxof( RcString value, int w, bool in_bytes ) {
    Variable res = scope->visit( names, value, true );

    //    Value res;
    //    Ref::MapInter mod;
    //    Vec<Ref::Break> breaks;
    //    Ref::intercept( mod, breaks, [&]() {
    //        Scope inter( scope );
    //        res = scope->visit( &inter, source_vec, value, true ).p_get( scope );
    //    }, true );

    // getsetter
    Type *type = res.type;
    if ( type->getsetter() )
        type = reinterpret_cast<GetSetter *>( res.ptr() )->get_type( scope );

    // typeof
    if ( w == 0 )
        return { vm, vm->type_Type, &type };

    // sizeof of aligof with a type
    if ( type == vm->type_SurdefList || type == vm->type_Type )
        type = res.apply( scope, true, {}, {}, false ).type;

    // sizeof of aligof with an instance
    SI32 vres = w == 1 ? type->size : type->alig;
    if ( in_bytes )
        vres = ( vres + 7 ) / 8;
    return { vm, vm->type_SI32, &vres };
}

Variable AstVisitorVm::assign( Scope *scope, RcString name, std::function<Variable()> rhs_func, PI8 flags ) {
    if ( scope->ctor_self ) {
        if ( flags & ASSIGN_FLAG_static )
            return scope->vm->ref_void;

        if ( scope->wpc && scope->wpc->count( name ) )
            return scope->vm->ref_void;

        // find attribute references
        auto iter_attr = scope->ctor_self.type->attributes.find( name );
        if ( iter_attr == scope->ctor_self.type->attributes.end() )
            return scope->add_error( "Attribute '{}' not registered in type", name ), scope->vm->ref_void;

        // call ctor recursively
        if ( iter_attr->second.type->has_a_constructor() ) {
            Variable attr = scope->ctor_self.sub_part( iter_attr->second.type, iter_attr->second.off_in_bits / 8 );
            Variable ctor = attr.find_attribute( scope, "construct" );

            if ( flags & ASSIGN_FLAG_type ) // ~=
                ctor.apply( scope, false );
            else // :=
                ctor.apply( scope, false, rhs_func() );

            return attr;
        }

        // complete size information
        // Value size = scope->ctor_room_inst->children[ 0 ];
        // Value alig = scope->ctor_room_inst->children[ 1 ];
        // Value va = attr.p_get( scope );
        // scope->ctor_room_inst->mod_child( 1, make_Lcm( alig, va.alig() ) );                         // alig = lcm ( alig, attr.alig );
        // scope->ctor_room_inst->mod_child( 0, make_Add( make_Ceil( size, va.alig() ), va.size() ) ); // size = ceil( size, attr.alig ) + attr.size;
        return scope->vm->ref_void;
    }


    Variable rhs = rhs_func();
    if ( flags & ASSIGN_FLAG_type )
        rhs = rhs.apply( scope, true, {}, {}, scope->type != Scope::Scope_type::TYPE_CTOR );
    else if ( ( rhs.content->cpt_use > 1 || rhs.type->getsetter() ) && ( flags & ASSIGN_FLAG_ref ) == 0 )
        rhs = rhs.find_attribute( scope, "operator :=" ).apply( scope, true ); // make a copy

    if ( scope->variables.count( name ) )
        scope->add_error( "Identifier '{}' has already been declared", name );
    if ( flags & ASSIGN_FLAG_const )
        rhs.flags |= Variable::Flags::CONST;

    Scope::VariableFlags vf = Scope::VariableFlags::NONE;
    if ( flags & ASSIGN_FLAG_static ) vf |= Scope::VariableFlags::STATIC;
    if ( flags & ASSIGN_FLAG_export ) vf |= Scope::VariableFlags::EXPORT;
    if ( flags & ASSIGN_FLAG_global ) vf |= Scope::VariableFlags::GLOBAL;

    scope->reg_var( name, rhs, vf );
    return rhs;
}

RcString AstVisitorVm::string_assembly( const Vec<RcString> &args ) {
    RcString name;
    for( const RcString &arg : args ) {
        Variable val = scope->visit( names, arg, true ).ugs( scope );
        if ( val.type != vm->type_String ) {
            val = vm->main_scope.find_variable( "String" ).apply( scope, true, val );
            if ( val.type != vm->type_String ) {
                if ( val.type != vm->type_Error )
                    scope->add_error( "conv to String did no work" );
                return {};
            }
        }
        String *n = rcast( val.ptr() );
        name = name + RcString( *n );
    }
    return name;
}