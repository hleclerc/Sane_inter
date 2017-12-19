#include "Variable.h"
#include "RefLeaf.h"
#include "Type.h"
#include "gvm.h"

Variable::Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset, Flags flags ) : ref( ref ), type( type ), flags( flags ), offset( offset ) {
}

Variable::Variable( const Value &value ) : Variable( new RefLeaf( value ), value.type ) {
}

Variable &Variable::operator=( const Variable &value ) {
    ref    = value.ref;
    type   = value.type;
    offset = value.offset;
    return *this;
}

bool Variable::error() const {
    return type == 0 || type->error();
}

void Variable::write_to_stream( std::ostream &os ) const {
    if ( ref )
        os << *ref;
    else
        os << "NULL";
}

RcString Variable::valid_constraint_for( const Variable &tested_var, TCI &tci ) const {
    return type->checks_type_constraint( *this, tested_var, tci );
}

Variable Variable::find_attribute( const RcString &name, bool ret_err, bool msg_if_err ) const {
    // data from the type
    if ( Variable res = type->find_attribute( name, *this, flags, offset )  )
        return res;

    // look for a 'self' function (external method), from the current scope to the deeper ones
    for( Scope *s = gvm->scope; s; s = s->parent_for_vars() ) {
        auto iter = s->variables.find( name );
        if ( iter != s->variables.end() && iter->second.flags & Scope::VariableFlags::SELF_AS_ARG )
            return iter->second.var.type->with_self( iter->second.var, *this );
    }

    // try with get_, set_, mod_, typeof_, or 'operator .'
    if ( name.begins_with( "get_" ) == false && name.begins_with( "set_" ) == false && name.begins_with( "mod_" ) == false && name.begins_with( "typeof_" ) == false && name != "operator ." ) {
        Variable g = find_attribute( "get_" + name, false );
        Variable s = find_attribute( "set_" + name, false );
        Variable m = find_attribute( "mod_" + name, false );
        if ( g || s || m ) {
            TODO;
            //            Variable type_of = find_attribute( scope, "typeof_" + name, false );
            //            Variable res( scope->vm, scope->vm->type_GetSetter );
            //            GetSetter *gs = rcast( res.ptr() );
            //            gs->type_of = type_of;
            //            gs->get = g;
            //            gs->set = s;
            //            gs->mod = m;
            //            return res;
        }

        if ( Variable op = find_attribute( "operator .", false ) ) {
            TODO;
            //            Variable str( scope->vm, scope->vm->type_String );
            //            reinterpret_cast<String *>( str.ptr() )->init( name );
            //            return op.apply( scope, true, str );
        }
    }

    if ( msg_if_err )
        gvm->add_error( "{} has no attribute {}", *type, name );
    return ret_err ? gvm->ref_error : Variable{};
}

void Variable::setup_vtables() {
    TODO;
}

Variable Variable::chbeba( bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    TODO; return {};
}

Variable Variable::select( bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    TODO; return {};
}

Variable Variable::apply( bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, ApplyFlags apply_flags, const Vec<size_t> &spreads ) {
    if ( spreads.size() ) {
        Vec<RcString> v_names( Rese(), names.size() );
        Vec<Variable> v_args( Rese(), args.size() );
        bool has_err = false;
        for( size_t i = 0, n = args.size() - names.size(); i < args.size(); ++i ) {
            if ( spreads.contains( i ) ) {
                if ( args[ i ].error() )
                    has_err = true;
                else
                    args[ i ].type->spread_in( args[ i ], v_args, v_names );
            } else {
                if ( i < n )
                    v_args.insert( v_args.size() - v_names.size(), args[ i ] );
                else {
                    v_names << names[ i - n ];
                    v_args << args[ i ];
                }
            }
        }
        return has_err ? gvm->ref_error : apply( want_ret, v_args, v_names, apply_flags );
    }

    return type->apply( *this, want_ret, args, names, {}, apply_flags );
}

String Variable::as_String() const {
    TODO;
    return {};
}

SI32 Variable::as_SI32() const {
    TODO;
    return {};
}

