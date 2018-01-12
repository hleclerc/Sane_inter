#include "RefGetsetter.h"
#include "gvm.h"

RefGetsetter::RefGetsetter( const Variable &g, const Variable &s, const Variable &m, const Variable &t ) : g( g ), s( s ), m( m ), t( t ) {
    if ( t )
        type = const_cast<Variable &>( t ).apply( true, {}, {}, ApplyFlags::DONT_CALL_CTOR ).type;
    else if ( g ) {
        value = const_cast<Variable &>( g ).apply( true );
        type = value.type;
    } else {
        gvm->add_error( "A getsetter must have either a typeof_... or a get_..." );
        type = 0;
    }
}

Variable RefGetsetter::intercept_find_attribute( const RcString &name, Type *var_type, bool is_const, SI32 var_offset ) const {
    if ( s && var_type == type && name == "operator =" )
        return s;
    return {};
}

void RefGetsetter::write_to_stream( std::ostream &os ) const {
    os << "getsetter(" << g << "," << s << "," << m << "," << t << ")";
}

bool RefGetsetter::is_shared() const {
    return true;
}

Variable RefGetsetter::variable() const {
    if ( ! value )
        value = const_cast<Variable &>( g ).apply( true );
    return value;
}

void RefGetsetter::constify() {
    variable();
    value.constify( true );
}

Value RefGetsetter::get() const {
    return variable().get();
}

void RefGetsetter::set( const Value &val, int cst ) {
    variable().memcpy( val, cst );
}
