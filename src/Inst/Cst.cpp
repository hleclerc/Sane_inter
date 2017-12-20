#include "../System/Memcpy.h"
#include "../Type.h"
#include "../gvm.h"
#include "Cst.h"

Cst::Cst( int size, void *val, void *kno ) : val( size ), kno( size ) {
    if ( val )
        memcpy( this->val.data, val, ( size + 7 ) / 8 );
    if ( kno )
        memcpy( this->kno.data, kno, ( size + 7 ) / 8 );
    else
        this->kno.set( val );
}

void Cst::write_dot( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    if ( type )
        type->write_cst( os, val.data + offset / 8, offset % 8 );
    else
        os << "UntypedCst(" << val << ")";
}

void Cst::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    if ( beg_src >= val.size )
        return;
    if ( beg_src + len > val.size )
        len = val.size - beg_src;
    memcpy_bit( dst, beg_dst, val.data, beg_src, len, msk );
    memset_bit( msk, beg_dst, false, len );
}

SI32 Cst::size() const {
    return val.size;
}

Value make_Cst_SI32( SI32 val ) {
    if ( gvm->reverse_endianness ) val = __builtin_bswap32( val );
    return { new Cst( 32, &val ), 0, gvm->type_SI32 };
}

Value make_Cst_Bool( Bool val ) {
    return { new Cst( 1, &val ), 0, gvm->type_Bool };
}

Value make_Cst( Type *type ) {
    return { new Cst( type->content.data.size ), 0, type };
}

