#include "AstVisitorFutureAttrs.h"
#include "DelayedVarInScope.h"
#include "Ast_visitor_Vm.h"
#include "System/rcast.h"
#include "GetSetter.h"
#include "Import.h"
#include "Vm.h"

Scope::Scope( Scope_type type, Scope *parent ) : parent( parent ), type( type ), vm( parent ? parent->vm : 0 ) {
    // valid_scope_ptr = 0;
    in_construction = 0;
    nb_breaks       = 0;
    nb_conts        = 0;
    last_var        = 0;
    import          = 0;
    root            = parent ? parent->root : 0;
    wpc             = 0;
}

Scope::Scope( Vm *vm ) : Scope( Scope_type::ROOT, 0 ) {
    this->vm = vm;
    root     = this;
}

Scope::~Scope() {
    for( const auto &d : delayed_vars )
        for( DelayedVarInScope *dv : d.second )
            dv->scope = 0;

    clear();

    if ( parent ) {
        parent->ret = ret;

        if ( nb_breaks )
            parent->nb_breaks = nb_breaks - 1;
        else if ( nb_conts )
            parent->nb_conts = nb_conts - 1;
    }
}

void Scope::add_error( int nb_calls_to_skip, const std::string &msg ) const {
    const Scope *s = this;
    while( nb_calls_to_skip ) {
        if ( ! s->parent )
            return s->add_error( msg );
        nb_calls_to_skip -= ( s->type == Scope::Scope_type::CALL );
        s = s->parent;
    }
    s->add_error( msg );
}

void Scope::add_error( const std::string &msg ) const {
    disp_error( prep_error( msg ) );
}

void Scope::disp_pos( const std::string &msg ) const {
    Error_list::Error error;
    bool disp = true;
    for( const Scope *s = this; s; s = s->parent ) {
        if ( s->pos.cur_names && disp ) {
            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
            disp = false;
        }

        if ( s->type == Scope::Scope_type::CALL )
            disp = true;
    }
    std::cerr << msg << ": " << error << std::endl;
}

Error_list::Error &Scope::prep_error( const std::string &msg ) const {
    Error_list::Error &error = vm->el.add( msg );
    bool disp = true;
    for( const Scope *s = this; s; s = s->parent ) {
        if ( s->pos.cur_names && disp ) {
            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
            // disp = false;
        }

        if ( s->type == Scope::Scope_type::CALL )
            disp = true;
    }
    return error;
}

void Scope::disp_error( const Error_list::Error &error ) const {
    std::cerr << error;
}

Variable Scope::find_variable( const Rc_string &name, bool ret_err, bool allow_ambiant, bool ret_z_if_in_self ) {
    // from the current scope to the deeper ones
    for( Scope *s = this; s; s = s->parent_for_vars() ) {
        if ( allow_ambiant == false && ! s->parent )
            break;

        // scope s
        auto iter = s->variables.find( name );
        if ( iter != s->variables.end() )
            return ret_z_if_in_self && ( s->type == Scope::Scope_type::TYPE_CTOR || s->ctor_self ) && ! ( iter->second.flags & ( VariableFlags::TEMPLATE | VariableFlags::STATIC | VariableFlags::CATCHED ) ) ? Variable{} : iter->second.var;

        // try in future attrs
        if ( s->futur_attrs.count( name ) ) {
            if ( ret_z_if_in_self && ( s->type == Scope::Scope_type::TYPE_CTOR || s->ctor_self ) )
                return {};

            Variable res( vm, vm->type_GetSetter );
            GetSetter *gs = rcast( res.ptr() );

            gs->get = { vm, vm->type_DelayedVarInScope };
            DelayedVarInScope *div = rcast( gs->get.ptr() );
            delayed_vars[ name ] << div;
            div->scope = this;
            div->name = name;

            return res;
        }

        //
        if ( s->self && s->self_method_name != "operator ." )
            if ( Variable res = s->self.find_attribute( s, name, false ) )
                return ret_z_if_in_self ? Variable {} : res;
    }

    // else, try in predefs
    if ( allow_ambiant ) {
        auto iter = vm->predefs.find( name );
        if ( iter != vm->predefs.end() )
            return iter->second;

        auto itef = vm->predeffs.find( name );
        if ( itef != vm->predeffs.end() )
            return itef->second( this );
    }

    //    // try with get_, set_, mod_, typeof_, or 'operator .'
    //    if ( name.begins_with( "get_" ) == false && name.begins_with( "set_" ) == false && name.begins_with( "mod_" ) == false && name.begins_with( "typeof_" ) == false && name != "operator ." ) {
    //        if ( name == "size" )
    //            ERROR("d");
    //        Variable g = find_variable( "get_" + name, false, allow_ambiant );
    //        Variable s = find_variable( "set_" + name, false, allow_ambiant );
    //        Variable m = find_variable( "mod_" + name, false, allow_ambiant );
    //        if ( g || s || m ) {
    //            Variable type_of = find_variable( "typeof_" + name, false, allow_ambiant );
    //            Variable res( vm, vm->type_GetSetter );
    //            GetSetter *gs = rcast( res.ptr() );
    //            gs->type_of = type_of;
    //            gs->get = g;
    //            gs->set = s;
    //            gs->mod = m;
    //            return res;
    //        }

    //        //        if ( Variable op = find_attribute( scope, "operator .", false ) ) {
    //        //            Variable str( scope->vm, scope->vm->type_String );
    //        //            reinterpret_cast<String *>( str.ptr() )->init( name );
    //        //            return op.apply( scope, true, str );
    //        //        }
    //    }

    // else, return an error Value
    return ret_err ? vm->ref_error : Variable{};
}

Scope::Exception Scope::exception( const Variable &var, bool silent ) {
    Error_list::Error error;
    error.msg = va_string( "Unhandled exception {}", var );
    for( const Scope *s = this; s; s = s->parent )
        if ( s->pos.cur_names )
            error.ac( s->src_name( s->pos.cur_src ), s->pos.cur_off );
    return { var, error, silent };
}

Variable Scope::visit( const Ast_crepr &ac, bool want_ret ) {
    return visit( ac.names, ac.code, want_ret );
}

Variable Scope::visit( const Rc_string &names, const Rc_string &code, bool want_ret ) {
    auto _ = raii_save( pos );
    Ast_visitor_Vm av( this, names, want_ret );
    ast_visit( av, code );
    return av.ret;
}

Variable Scope::visit( const Rc_string &names, const Vec<Rc_string> &code, bool want_ret ) {
    auto _ = raii_save( pos );

    // futur_attrs
    AstVisitorFutureAttrs af( futur_attrs );
    for( const Rc_string &c : code )
        ast_visit( af, c );

    //
    if ( code.size() ) {
        for( size_t i = 0; i < code.size() - 1; ++i )
            visit( names, code[ i ], false );
        return visit( names, code.back(), want_ret );
    }
    ASSERT( want_ret == false, "..." );
    return {};
}

Variable Scope::visit( const Vec<Ast_crepr> &creps, bool want_ret ) {
    auto _ = raii_save( pos );

    // futur_attrs
    AstVisitorFutureAttrs af( futur_attrs );
    for( const Ast_crepr &ac : creps )
        ast_visit( af, ac.code );

    //
    if ( creps.size() ) {
        for( size_t i = 0; i < creps.size() - 1; ++i )
            visit( creps[ i ], false );
        return visit( creps.back(), want_ret );
    }
    ASSERT( want_ret == false, "..." );
    return {};
}

Rc_string Scope::src_name( size_t index ) const {
    Rc_string cp_cn = pos.cur_names;
    Hpipe::BinStream<Rc_string> bs( &cp_cn );
    size_t size = bs.read_unsigned();
    if ( index >= size )
        return {};
    for( ; index; --index )
        bs.skip_string();
    return bs.read_String();
}

Variable Scope::find_self( bool ret_err ) {
    // from the current scope to the deeper ones
    for( Scope *s = this; s; s = s->parent_for_vars() )
        if ( s->self )
            return s->self;
    return ret_err ? vm->ref_error : Variable{};
}

void Scope::reg_var( const Rc_string &name, const Variable &var, Scope::VariableFlags flags, bool check ) {
    //
    if ( import && flags & VariableFlags::EXPORT )
        import->exports << Import::Export{ name, var };
    // global
    if ( flags & VariableFlags::GLOBAL )
        return root->reg_var( name, var, flags & ~ VariableFlags::GLOBAL );
    // already registered ?
    bool already_present = variables.count( name );
    if ( already_present && check ) {
        if ( ! ( flags & Scope::VariableFlags::CATCHED ) ) {
            add_error( "Variable '{}' is already defined in current scope", name );
            ERROR(".");
        }
        return;
    }

    // completion of futur_attrs
    auto iter = delayed_vars.find( name );
    if ( iter != delayed_vars.end() )
        for( DelayedVarInScope *dv : iter->second )
            dv->value = var;
    // -> make a record
    NV &nv = variables[ name ];
    nv.flags = flags;
    nv.name  = name;
    nv.var   = var;

    if ( ! already_present ) {
        nv.prev  = last_var;
        last_var = &nv;
    }
}

Variable *Scope::add_static_variable( const Variable &var ) {
    NV *nv = static_variables.new_back_item();
    nv->var = var;

    nv->prev  = last_var;
    last_var = nv;

    return &nv->var;
}

size_t Scope::nb_scopes_to_catch() const {
    size_t nb_l = 1;
    for( const Scope *s = this; s && s->type != Scope_type::TRY; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_break() const {
    const Scope *loop = this;
    while ( loop->type != Scope_type::WHILE && loop->type != Scope_type::FOR_BEG ) {
        loop = loop->parent_interp( false );
        if ( ! loop )
            return add_error( "break must be called inside a loop (while, for, ...)" ), 0;
    }

    size_t nb_l = 1;
    for( const Scope *s = this; s != loop; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_cont() const {
    const Scope *loop = this;
    while ( loop->type != Scope_type::WHILE && loop->type != Scope_type::FOR_EXE ) {
        loop = loop->parent_interp( false );
        if ( ! loop )
            return add_error( "continue must be called inside a loop (while, for, ...)" ), 0;
    }

    size_t nb_l = 1;
    for( const Scope *s = this; s != loop; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_ret() const {
    const Scope *caller = this;
    while ( caller->type != Scope_type::CALL )
        caller = caller->parent_interp( false );

    size_t nb_l = 1;
    for( const Scope *s = this; s != caller; s = s->parent )
        ++nb_l;
    return nb_l;
}

Scope *Scope::parent_interp( bool squeeze_for_beg ) const {
    if ( type == Scope_type::FOR_EXE ) {
        int nb_for = 1;
        for( Scope *s = parent; s; s = s->parent ) {
            if ( s->type == Scope_type::FOR_BEG ) {
                if ( --nb_for == 0 )
                    return squeeze_for_beg ? s->parent : s;
            } else if ( s->type == Scope_type::FOR_EXE )
                ++nb_for;
        }
    }
    if ( type == Scope_type::IF_EXE )
        return parent->parent;
    return parent;
}

void Scope::clear() {
    for( NV *nv = last_var; nv; nv = nv->prev )
        nv->var.clear();
}
