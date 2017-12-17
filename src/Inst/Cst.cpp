#include "../Type.h"
#include "../gvm.h"
#include "Cst.h"

Cst::Cst( int size, void *val, void *kno ) : val( size ), kno( size ) {
    memcpy( this->val.data, val, ( size + 7 ) / 8 );
    if ( kno )
        memcpy( this->kno.data, kno, ( size + 7 ) / 8 );
    else
        this->kno.set( val );
}

void Cst::write_dot( std::ostream &os, Type *type, int offset ) const {
    type->write_cst( os, val.data + offset / 8, offset % 8 );
}

Value make_Cst_SI32( SI32 val ) {
    if ( gvm->reverse_endianness )
        val = __builtin_bswap32( val );
    return { new Cst( 32, &val ), gvm->type_SI32 };
}

