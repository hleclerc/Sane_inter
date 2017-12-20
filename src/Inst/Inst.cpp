#include "../Value.h"

Inst::~Inst() {
}

void Inst::add_child( const Value &ch ) {
    children << ch;
}

void Inst::write_to_stream( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    write_dot( os, nout, type, offset );
}

void Inst::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    write_dot( std::cerr );
    TODO;
}

SI32 Inst::size() const {
    write_dot( std::cerr );
    TODO;
    return 0;
}
