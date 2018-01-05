#include "WriteFd.h"

WriteFd::WriteFd( const Vec<Value> &args ) {
    for( const Value &arg : args )
        add_child( arg );
}

void WriteFd::write_dot( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    os << "WriteFd";
}

int WriteFd::inp_corr( int nout ) const {
    return 3;
}

