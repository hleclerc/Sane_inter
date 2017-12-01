#include "System/rcast.h"
#include "Type_Varargs.h"

Type_Varargs::Type_Varargs() : Type_BaseBin( "Varargs" ) {
}

void Type_Varargs::spread_in( Scope *scope, const Variable &self, Vec<Variable> &values, Vec<Rc_string> &names ) {
    Varargs *va = rcast( self.ptr() );
    for( size_t i = 0; i < va->values.size() - va->names.size(); ++i )
        values.insert( values.size() - names.size(), va->values[ i ] );
    for( size_t i = 0, n = va->values.size() - va->names.size(); i < va->names.size(); ++i ) {
        values << va->values[ n + i ];
        names << va->names[ i ];
    }
}

void Type_Varargs::write_to_stream( std::ostream &os, const char *content ) const {
    int cpt = 0;
    const Varargs *va = rcast( content );
    for( size_t i = 0; i < va->values.size() - va->names.size(); ++i )
        os << ( cpt++ ? " " : "" ) << va->values[ i ];
    for( size_t i = 0, n = va->values.size() - va->names.size(); i < va->names.size(); ++i )
        os << ( cpt++ ? " " : "" ) << va->names[ i ] << "=" << va->values[ n + i ];
}
