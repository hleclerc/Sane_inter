#include "../Codegen/Codegen.h"
#include "WriteFd.h"

WriteFd::WriteFd( const Vec<Value> &args ) {
    for( const Value &arg : args )
        add_child( arg );
}

WriteFd::WriteFd( AttrClone, const WriteFd * ) {
}

void WriteFd::write_code( StreamSep &ss, Codegen &cg ) {
    ss.write_beg();
    if ( children.size() == 2 ) {
        cg.write_fd_repr( *ss, children[ 1 ].type );
        *ss << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << ")";
    } else {
        cg.write_fd_repr( *ss, 0 );
        *ss << "(" << cg.repr( children[ 0 ] ) << "," << cg.repr( children[ 1 ] ) << "," << cg.repr( children[ 2 ] ) << ")";
    }
    ss.write_end( ";" );
}

void WriteFd::write_dot( std::ostream &os ) const {
    os << "WriteFd";
}

int WriteFd::inp_corr( int nout ) const {
    return 3;
}

