#include "System/rcast.h"
#include "GetSetter.h"
#include "Variable.h"
#include "String.h"
#include "Scope.h"
#include "Class.h"
#include "Type.h"
#include "TCI.h"
#include "Vm.h"
#include <string.h>

Variable::Variable( Rc_ptr<Content,true> content, Variable::Flags flags, Type *type, SI32 offB ) : content( content ), flags( flags ), type( type ), offB( offB ) {
}

Variable::Variable( Vm *vm, Type *type, void *value, Content::Flags content_flags, Flags flags ) : flags( flags ), type( type ) {
    _reserve( vm, type, content_flags, ( type->size + 7 ) / 8 );
    offB = 0;

    type->bin_ctor( &vm->main_scope, *this );
    if ( value )
        memcpy( content->data, value, ( type->size + 7 ) / 8 );
}

Variable::Variable( const Variable &var ) : content( var.content ), flags( var.flags ), type( var.type ), offB( var.offB ) {
}

Variable::Variable( Variable &&var ) : content( std::move( var.content ) ), flags( var.flags ), type( var.type ), offB( var.offB ) {
}

Variable::Variable() : type( 0 ) {
}

Variable &Variable::operator=( const Variable &var ) {
    content = var.content;
    flags   = var.flags  ;
    type    = var.type   ;
    offB    = var.offB   ;
    return *this;
}

Variable &Variable::operator=( Variable &&var ) {
    content = std::move( var.content );
    flags   = var.flags;
    type    = var.type;
    offB    = var.offB;
    return *this;
}

Rc_string Variable::valid_constraint_for( Scope *scope, const Variable &tested_var, TCI &tci ) const {
    return type->checks_type_constraint( scope, *this, tested_var, tci );
}

Variable::operator bool() const {
    return content;
}

void Variable::write_to_stream( std::ostream &os ) const {
    if ( content )
        type->write_to_stream( os, content->data + offB );
    else
        os << "null";
}

Variable Variable::find_attribute( Scope *scope, const Rc_string &name, bool ret_err, bool msg_if_err ) const {
    // data from the type
    if ( Variable res = type->find_attribute( scope, name, this, 0, content.ptr(), flags, offB )  )
        return res;

    // look for a 'self' function (external method), from the current scope to the deeper ones
    for( Scope *s = scope; s; s = s->parent_for_vars() ) {
        auto iter = s->variables.find( name );
        if ( iter != s->variables.end() && iter->second.flags & Scope::VariableFlags::SELF_AS_ARG )
            return iter->second.var.type->with_self( scope, iter->second.var, *this );
    }

    // try with get_, set_, mod_, typeof_, or 'operator .'
    if ( name.begins_with( "get_" ) == false && name.begins_with( "set_" ) == false && name.begins_with( "mod_" ) == false && name.begins_with( "typeof_" ) == false && name != "operator ." ) {
        Variable g = find_attribute( scope, "get_" + name, false );
        Variable s = find_attribute( scope, "set_" + name, false );
        Variable m = find_attribute( scope, "mod_" + name, false );
        if ( g || s || m ) {
            Variable type_of = find_attribute( scope, "typeof_" + name, false );
            Variable res( scope->vm, scope->vm->type_GetSetter );
            GetSetter *gs = rcast( res.ptr() );
            gs->type_of = type_of;
            gs->get = g;
            gs->set = s;
            gs->mod = m;
            return res;
        }

        if ( Variable op = find_attribute( scope, "operator .", false ) ) {
            Variable str( scope->vm, scope->vm->type_String );
            reinterpret_cast<String *>( str.ptr() )->init( name );
            return op.apply( scope, true, str );
        }
    }

    if ( msg_if_err )
        scope->add_error( "{} has no attribute {}", *type, name );
    return ret_err ? scope->vm->ref_error : Variable{};
}

bool Variable::get_PT_value( Scope *scope, PT &val ) const {
    if ( type == scope->vm->type_PT   ) { val = *reinterpret_cast<PT   *>( ptr() ); return true; }
    if ( type == scope->vm->type_ST   ) { val = *reinterpret_cast<ST   *>( ptr() ); return true; }
    if ( type == scope->vm->type_PI64 ) { val = *reinterpret_cast<PI64 *>( ptr() ); return true; }
    if ( type == scope->vm->type_SI64 ) { val = *reinterpret_cast<SI64 *>( ptr() ); return true; }
    if ( type == scope->vm->type_PI32 ) { val = *reinterpret_cast<PI32 *>( ptr() ); return true; }
    if ( type == scope->vm->type_SI32 ) { val = *reinterpret_cast<SI32 *>( ptr() ); return true; }
    if ( type == scope->vm->type_PI16 ) { val = *reinterpret_cast<PI16 *>( ptr() ); return true; }
    if ( type == scope->vm->type_SI16 ) { val = *reinterpret_cast<SI16 *>( ptr() ); return true; }
    if ( type == scope->vm->type_PI8  ) { val = *reinterpret_cast<PI8  *>( ptr() ); return true; }
    if ( type == scope->vm->type_SI8  ) { val = *reinterpret_cast<SI8  *>( ptr() ); return true; }
    if ( type == scope->vm->type_Bool ) return *reinterpret_cast<Bool *>( ptr() );
    PE( *type );
    TODO; // call a constructor
    return false;
}

void Variable::setup_vtables() {
    int num_vtable = 0;
    type->apply_on_vtables( [&]( Type *super_type, SI32 offB ) {
        *reinterpret_cast<Vtable **>( ptr() + offB ) = &type->vtables[ num_vtable++ ];
    } );
}

Variable Variable::sub_part( Type *type, SI32 offset ) {
    return { content, flags, type, offB + offset };
}

char *Variable::_reserve( Vm *vm, Type *type, Content::Flags content_flags, size_t rese_in_bytes ) {
    content = Variable::Content::alloc( rese_in_bytes, vm, type, content_flags );
    return content->data;
}

bool Variable::is_false( Scope *scope ) {
    return type->is_false( scope, *this );
}

bool Variable::is_true( Scope *scope ) {
    return type->is_true( scope, *this );
}

Variable Variable::chbeba( Scope *scope, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    return type->chbeba( scope, *this, want_ret, args, names );
}

Variable Variable::select( Scope *scope, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    return type->select( scope, *this, want_ret, args, names );
}

FP64 Variable::as_FP64( Scope *scope ) const {
    if ( type == scope->vm->type_PT   ) return *reinterpret_cast<PT   *>( ptr() );
    if ( type == scope->vm->type_ST   ) return *reinterpret_cast<ST   *>( ptr() );
    if ( type == scope->vm->type_PI64 ) return *reinterpret_cast<PI64 *>( ptr() );
    if ( type == scope->vm->type_SI64 ) return *reinterpret_cast<SI64 *>( ptr() );
    if ( type == scope->vm->type_PI32 ) return *reinterpret_cast<PI32 *>( ptr() );
    if ( type == scope->vm->type_SI32 ) return *reinterpret_cast<SI32 *>( ptr() );
    if ( type == scope->vm->type_PI16 ) return *reinterpret_cast<PI16 *>( ptr() );
    if ( type == scope->vm->type_SI16 ) return *reinterpret_cast<SI16 *>( ptr() );
    if ( type == scope->vm->type_PI8  ) return *reinterpret_cast<PI8  *>( ptr() );
    if ( type == scope->vm->type_SI8  ) return *reinterpret_cast<SI8  *>( ptr() );
    if ( type == scope->vm->type_Bool ) return *reinterpret_cast<Bool *>( ptr() );
    PE( *type );
    P( *type );
    TODO; // call a constructor
    return 0;
}

Variable Variable::apply( Scope *scope, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Vec<size_t> &spreads ) {
    if ( spreads.size() ) {
        Vec<Rc_string> v_names( Rese(), names.size() );
        Vec<Variable> v_args( Rese(), args.size() );
        bool has_err = false;
        for( size_t i = 0, n = args.size() - names.size(); i < args.size(); ++i ) {
            Variable ref = args[ i ].ugs( scope );
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
        return has_err ? scope->vm->ref_error : apply( scope, want_ret, v_args, v_names, call_ctor );
    }

    return type->apply( scope, *this, want_ret, args, names, call_ctor );
}

bool Variable::error() const {
    return type && type->error();
}

SI32 Variable::size() const {
    return type->size;
}

SI32 Variable::alig() const {
    return type->alig;
}

char *Variable::ptr() const {
    return content->data + offB;
}

Type *Variable::ugs_type( Scope *scope, Variable *get_res ) const {
    return type->getsetter() ? reinterpret_cast<GetSetter *>( ptr() )->get_type( scope, get_res ) : type;
}

Variable Variable::ugs( Scope *scope ) const {
    if ( type && type->getsetter() ) {
        GetSetter *gs = rcast( ptr() );
        return gs->get_var( scope );
    }
    return *this;
}

Variable::Content::~Content() {
    if ( flags & Flags::NOT_CONSTRUCTED )
        return;

    // object is going to be destroyed anyway (but we don't want to destroy it twice)
    cpt_use = 987;

    // call the destructor
    if ( type ) {
        Variable var( this, Variable::Flags::NONE, type, 0 );
        var.type->destroy( &vm->main_scope, var );
    }
}
