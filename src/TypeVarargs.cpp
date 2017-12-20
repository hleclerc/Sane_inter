#include "TypeVarargs.h"
#include "Varargs.h"

TypeVarargs::TypeVarargs() : Type( "Varargs" ) {
}

void TypeVarargs::spread_in( const Variable &self, Vec<Variable> &values, Vec<RcString> &names ) {
    Varargs *va = self.rcast<Varargs>();
    for( size_t i = 0; i < va->values.size() - va->names.size(); ++i )
        values.insert( values.size() - names.size(), va->values[ i ] );
    for( size_t i = 0, n = va->values.size() - va->names.size(); i < va->names.size(); ++i ) {
        values << va->values[ n + i ];
        names << va->names[ i ];
    }
}

//void Type_Varargs::write_to_stream( std::ostream &os, const char *content ) const {
//    int cpt = 0;
//    const Varargs *va = rcast( content );
//    for( size_t i = 0; i < va->values.size() - va->names.size(); ++i )
//        os << ( cpt++ ? " " : "" ) << va->values[ i ];
//    for( size_t i = 0, n = va->values.size() - va->names.size(); i < va->names.size(); ++i )
//        os << ( cpt++ ? " " : "" ) << va->names[ i ] << "=" << va->values[ n + i ];
//}
