#include "../Value.h"

Inst::~Inst() {
}

void Inst::add_child( const Value &ch ) {
    children << ch;
}

void Inst::add_dep( const RcPtr<Inst> &inst ) {
    deps << inst;
}

void Inst::write_to_stream( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    write_dot( os, nout, type, offset );
    if ( children.size() ) {
        os << '(' << children[ 0 ];
        for( size_t i = 1; i < children.size(); ++i )
            os << ',' << children[ i ];
        os << ')';
    }
}

void Inst::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    write_dot( std::cerr );
    TODO;
}

void *Inst::rcast( SI32 nout, Type *type, SI32 offset ) {
    write_dot( std::cerr );
    TODO;
    return 0;
}

