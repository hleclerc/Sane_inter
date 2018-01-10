#include "../Type.h"
#include "Conv.h"

Conv::Conv( const Value &orig, Type *target_type ) : target_type( target_type ) {
    add_child( orig );
}

void Conv::write_dot( std::ostream &os ) const {
    os << "Conv[" << *target_type << "]";
}

void Conv::get_bytes( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const {
    TODO;
}

Value make_Conv( const Value &orig, Type *target_type ) {
    return { new Conv( orig, target_type ), 0, target_type };
}
