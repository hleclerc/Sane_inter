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

void RefGetsetter::write_to_stream( std::ostream &os ) const {
    os << "getsetter(" << g << "," << s << "," << m << "," << t << ")";
}

bool RefGetsetter::is_shared() const {
    return true;
}

Value RefGetsetter::get() const {
    if ( ! value )
        value = const_cast<Variable &>( g ).apply( true );
    return value.get();
}
