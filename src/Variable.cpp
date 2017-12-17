#include "Variable.h"
#include "RefLeaf.h"
#include "Type.h"
#include "gvm.h"

Variable::Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset ) : ref( ref ), type( type ), offset( offset ) {
}

Variable::Variable( const Value &value ) : Variable( new RefLeaf( value ), value.type, 0 ) {
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

Variable Variable::find_attribute( const RcString &name, bool ret_err, bool msg_if_err ) const {
    TODO; return {};
}

Variable Variable::chbeba(bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names) {
    TODO; return {};
}

Variable Variable::select(bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names) {
    TODO; return {};
}

Variable Variable::apply(bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, ApplyFlags apply_flags, const Vec<size_t> &spreads) {
    if ( spreads.size() ) {
        Vec<RcString> v_names( Rese(), names.size() );
        Vec<Variable> v_args( Rese(), args.size() );
        bool has_err = false;
        for( size_t i = 0, n = args.size() - names.size(); i < args.size(); ++i ) {
            Variable ref = args[ i ].ugs();
            if ( spreads.contains( i ) ) {
                if ( ref.error() )
                    has_err = true;
                else
                    ref.type->spread_in( ref, v_args, v_names );
            } else {
                if ( i < n )
                    v_args.insert( v_args.size() - v_names.size(), ref );
                else {
                    v_names << names[ i - n ];
                    v_args << ref;
                }
            }
        }
        return has_err ? gvm->ref_error : apply( want_ret, v_args, v_names, apply_flags );
    }

    return type->apply( *this, want_ret, args, names, apply_flags );
}

Variable Variable::ugs() const {
    if ( type && type->getsetter() ) {
        TODO;
        //        GetSetter *gs = rcast( ptr() );
        //        return gs->get_var( scope );
    }
    return *this;

}

