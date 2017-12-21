#include "MemcpyKV.h"
#include "../Type.h"

MemcpyKV::MemcpyKV( const Value &dst, const Value &src, SI32 dst_off ) : dst_off( dst_off ) {
    add_child( dst );
    add_child( src );
}

void MemcpyKV::write_dot( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    os << "MemcpyKV[" << dst_off << "]";
}

void MemcpyKV::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    TODO;
}

Value make_MemcpyKV( const Value &dst, const Value &src, SI32 dst_off ) {
    return { new MemcpyKV( dst, src, dst_off ), 0, dst.type };
}
