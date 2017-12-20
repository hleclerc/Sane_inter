#include "DelayedVarInScope.h"
#include "System/RaiiSave.h"
#include <Hpipe/BinStream.h>
#include "Import.h"
#include "Scope.h"
#include "gvm.h"

Scope::Scope( Scope::ScopeType type ) : parent( gvm ? gvm->scope : 0 ), type( type ) {
    in_construction = 0;
    last_var        = 0;
    import          = 0;
    root            = parent ? parent->root : this;
    wpc             = 0;

    if ( gvm )
        gvm->scope = this;
}

Scope::~Scope() {
    for( const auto &d : delayed_vars )
        for( DelayedVarInScope *dv : d.second )
            dv->scope = 0;

    //    clear();

    //    if ( parent ) {
    //        parent->ret = ret;

    //        if ( nb_breaks )
    //            parent->nb_breaks = nb_breaks - 1;
    //        else if ( nb_conts )
    //            parent->nb_conts = nb_conts - 1;
    //    }

    if ( gvm )
        gvm->scope = parent;
}

Variable Scope::find_variable( const RcString &name, bool ret_err, bool allow_ambiant, bool ret_z_if_in_self ) {
    // from the current scope to the deeper ones
    for( Scope *s = this; s; s = s->parent_for_vars() ) {
        if ( allow_ambiant == false && ! s->parent )
            break;

        // scope s
        auto iter = s->variables.find( name );
        if ( iter != s->variables.end() )
            return ret_z_if_in_self && ( s->type == Scope::ScopeType::TYPE_CTOR || s->ctor_self ) && ! ( iter->second.flags & ( VariableFlags::TEMPLATE | VariableFlags::STATIC | VariableFlags::CATCHED ) ) ? Variable{} : iter->second.var;

        // try in future attrs
        if ( s->futur_attrs.count( name ) ) {
            TODO;
            //            if ( ret_z_if_in_self && ( s->type == Scope::ScopeType::TYPE_CTOR || s->ctor_self ) )
            //                return {};

            //            Variable res( vm, vm->type_GetSetter );
            //            GetSetter *gs = rcast( res.ptr() );

            //            gs->get = { vm, vm->type_DelayedVarInScope };
            //            DelayedVarInScope *div = rcast( gs->get.ptr() );
            //            delayed_vars[ name ] << div;
            //            div->scope = this;
            //            div->name = name;

            //            return res;
        }

        //
        if ( s->self && s->self_method_name != "operator ." )
            if ( Variable res = s->self.find_attribute( name, false ) )
                return ret_z_if_in_self ? Variable {} : res;
    }

    // else, try in predefs
    if ( allow_ambiant ) {
        auto iter = gvm->predefs.find( name );
        if ( iter != gvm->predefs.end() )
            return iter->second;

        auto itef = gvm->predeffs.find( name );
        if ( itef != gvm->predeffs.end() )
            return itef->second();
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
    return ret_err ? gvm->ref_error : Variable{};
}

Variable Scope::find_self( bool ret_err ) {
    // from the current scope to the deeper ones
    for( Scope *s = this; s; s = s->parent_for_vars() )
        if ( s->self )
            return s->self;
    return ret_err ? gvm->ref_error : Variable{};
}

void Scope::reg_var( const RcString &name, const Variable &var, Scope::VariableFlags flags, bool check ) { //
    if ( import && flags & VariableFlags::EXPORT )
        import->exports << Import::Export{ name, var };
    // global
    if ( flags & VariableFlags::GLOBAL )
        return root->reg_var( name, var, flags & ~ VariableFlags::GLOBAL );
    // already registered ?
    bool already_present = variables.count( name );
    if ( already_present && check ) {
        if ( ! ( flags & Scope::VariableFlags::CATCHED ) ) {
            gvm->add_error( "Variable '{}' is already defined in current scope", name );
            ERROR( "." );
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

size_t Scope::nb_scopes_to_catch() const {
    size_t nb_l = 1;
    for( const Scope *s = this; s && s->type != ScopeType::TRY; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_break() const {
    const Scope *loop = this;
    while ( loop->type != ScopeType::WHILE && loop->type != ScopeType::FOR_BEG ) {
        loop = loop->parent_interp( false );
        if ( ! loop )
            return gvm->add_error( "break must be called inside a loop (while, for, ...)" ), 0;
    }

    size_t nb_l = 1;
    for( const Scope *s = this; s != loop; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_cont() const {
    const Scope *loop = this;
    while ( loop->type != ScopeType::WHILE && loop->type != ScopeType::FOR_EXE ) {
        loop = loop->parent_interp( false );
        if ( ! loop )
            return gvm->add_error( "continue must be called inside a loop (while, for, ...)" ), 0;
    }

    size_t nb_l = 1;
    for( const Scope *s = this; s != loop; s = s->parent )
        ++nb_l;
    return nb_l;
}

size_t Scope::nb_scopes_to_ret() const {
    const Scope *caller = this;
    while ( caller->type != ScopeType::CALL )
        caller = caller->parent_interp( false );

    size_t nb_l = 1;
    for( const Scope *s = this; s != caller; s = s->parent )
        ++nb_l;
    return nb_l;
}

Scope *Scope::parent_interp( bool squeeze_for_beg ) const {
    if ( type == ScopeType::FOR_EXE ) {
        int nb_for = 1;
        for( Scope *s = parent; s; s = s->parent ) {
            if ( s->type == ScopeType::FOR_BEG ) {
                if ( --nb_for == 0 )
                    return squeeze_for_beg ? s->parent : s;
            } else if ( s->type == ScopeType::FOR_EXE )
                ++nb_for;
        }
    }
    if ( type == ScopeType::IF_EXE )
        return parent->parent;
    return parent;
}

Variable *Scope::add_static_variable(const Variable &var) {
    NV *nv = static_variables.new_back_item();
    nv->var = var;

    nv->prev  = last_var;
    last_var = nv;

    return &nv->var;
}

RcString Scope::Position::src_name( size_t index ) const {
    RcString cp_cn = cur_names;
    Hpipe::BinStream<RcString> bs( &cp_cn );
    size_t size = bs.read_unsigned();
    if ( index >= size )
        return {};
    for( ; index; --index )
        bs.skip_string();
    return bs.read_String();
}

RcString Scope::Position::src_name() const {
    return src_name( cur_src );
}
