#include "System/BoolVec.h"
#include "Inst/MemcpyKV.h"
#include "RefGetsetter.h"
#include "Variable.h"
#include "RefLeaf.h"
#include "Type.h"
#include "gvm.h"

Variable::Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset, Flags flags ) : ref( ref ), type( type ), flags( flags ), offset( offset ) {
}

Variable::Variable( const Value &value, Flags flags ) : Variable( new RefLeaf( value ), value.type, 0, flags ) {
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

bool Variable::is_shared() const { return ref->is_shared(); }

Variable Variable::to_Bool() const {
    if ( type != gvm->type_Bool ) {
        Variable cond_func = gvm->scope->find_variable( "Bool" );
        Variable cond_res = cond_func.apply( true, *this );
        ASSERT( cond_res.type == gvm->type_Bool, "..." );
        return cond_res;
    }

    return *this;
}

Variable Variable::equal( const Variable &that ) const {
    return find_attribute( "operator ==" ).apply( true, that );
}

bool Variable::is_false() const {
    if ( type != gvm->type_Bool )
        return to_Bool().is_false();
    Bool data;
    return get_bytes( &data, 0, 0, 1 ) && data == false;
}

bool Variable::is_true() const {
    if ( type != gvm->type_Bool )
        return to_Bool().is_false();
    Bool data;
    return get_bytes( &data, 0, 0, 1 ) && data == true;
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
    //
    if ( Variable res = ref->intercept_find_attribute( name, type, bool( flags & Flags::CONST ), offset ) )
        return res;

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
            RefGetsetter *rf = new RefGetsetter( g, s, m, find_attribute( "typeof_" + name, false ) );
            return { rf, rf->type };
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
    // TODO;
}

Variable Variable::constify( bool deep ) {
    flags |= Flags::CONST;

    if ( deep )
        ref->constify();

    return *this;
}

Variable Variable::chbeba( bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    return type->chbeba( *this, want_ret, args, names );
}

Variable Variable::select( bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names ) {
    return type->select( *this, want_ret, args, names );
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

FP64 Variable::as_FP64() const {
    TODO;
    return {};
}

SI32 Variable::as_SI32() const {
    TODO;
    return {};
}

Value Variable::get() const {
    Value res = ref->get();
    return { res.inst, res.nout, type, res.offset + offset };
}

bool Variable::get_bytes( void *dst, PI32 beg_dst, PI32 beg_src, PI32 len ) const {
    BoolVec msk( len, true );
    get_bytes( dst, beg_dst, beg_src, len, msk.data );
    return msk.all_false();
}

void Variable::get_bytes( void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    return get().get_bytes( dst, beg_dst, beg_src, len, msk );
}

bool Variable::get_value( SI32 &val ) const {
    #define TPT( BA, SI ) if ( type == gvm->type_##BA##SI ) { BA##SI v; if ( ! get_bytes( &v, 0, 0, SI ) ) return false; if ( gvm->reverse_endianness ) TODO; val = v; return true; }
    TPT( PI,  8 )
    TPT( SI,  8 )
    TPT( PI, 16 )
    TPT( SI, 16 )
    TPT( PI, 32 )
    TPT( SI, 32 )
    TPT( PI, 64 )
    TPT( SI, 64 )
    #undef TPT

    if ( type == gvm->type_PT ) { TODO; return true; }
    if ( type == gvm->type_ST ) { TODO; return true; }

    return gvm->scope->find_variable( "SI32" ).get_value( val );
}

void Variable::set_bv( const Value &src_val, int cst ) {
    if ( flags & Flags::CONST ) {
        gvm->add_error( "Const variable, should not be modified" );
        return;
    }
    ASSERT( ref, "..." );
    ref->set( src_val, cst );
}

void Variable::memcpy( const Value &src_val, int cst ) {
    set_bv( make_MemcpyKV( ref->get(), src_val, offset ), cst );
}

Variable Variable::sub_part( Type *new_type, SI32 add_off ) const {
    return { ref, new_type, offset + add_off, flags };
}

