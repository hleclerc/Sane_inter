#include "../System/byte_swap.h"
#include "../System/Memcpy.h"
#include "../Type.h"
#include "../gvm.h"
#include "Cst.h"

Cst::Cst( Type *type, int size, void *val, void *kno ) : type( type ), val( size ), kno( size ) {
    if ( val )
        memcpy( this->val.data, val, ( size + 7 ) / 8 );
    else
        this->val.set( false );
    if ( kno )
        memcpy( this->kno.data, kno, ( size + 7 ) / 8 );
    else
        this->kno.set( bool( val ) );
}

void Cst::write_to_stream( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    type->write_cst( os, val.data + offset / 8, offset % 8 );
}

void Cst::write_dot( std::ostream &os ) const {
    type->write_cst( os, val.data, 0 );
}

void Cst::get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    if ( beg_src >= val.size )
        return;
    if ( beg_src + len > val.size )
        len = val.size - beg_src;
    memcpy_bit( dst, beg_dst, val.data, beg_src, len, msk );
    memset_bit( msk, beg_dst, false, len );
}

int Cst::nb_outputs() const {
    return 1;
}

Value make_Cst_PI64( PI64 val ) { if ( gvm->reverse_endianness ) val = byte_swaped( val ); return { new Cst( gvm->type_PI64, 64, &val ), 0, gvm->type_PI64 }; }
Value make_Cst_SI64( SI64 val ) { if ( gvm->reverse_endianness ) val = byte_swaped( val ); return { new Cst( gvm->type_SI64, 64, &val ), 0, gvm->type_SI64 }; }
Value make_Cst_PI32( PI32 val ) { if ( gvm->reverse_endianness ) val = byte_swaped( val ); return { new Cst( gvm->type_PI32, 32, &val ), 0, gvm->type_PI32 }; }
Value make_Cst_SI32( SI32 val ) { if ( gvm->reverse_endianness ) val = byte_swaped( val ); return { new Cst( gvm->type_SI32, 32, &val ), 0, gvm->type_SI32 }; }
Value make_Cst_PI16( PI16 val ) { if ( gvm->reverse_endianness ) val = byte_swaped( val ); return { new Cst( gvm->type_PI16, 16, &val ), 0, gvm->type_PI16 }; }
Value make_Cst_SI16( SI16 val ) { if ( gvm->reverse_endianness ) val = byte_swaped( val ); return { new Cst( gvm->type_SI16, 16, &val ), 0, gvm->type_SI16 }; }
Value make_Cst_PI8 ( PI8  val ) { return { new Cst( gvm->type_PI8 , 8, &val ), 0, gvm->type_PI8  }; }
Value make_Cst_SI8 ( SI8  val ) { return { new Cst( gvm->type_SI8 , 8, &val ), 0, gvm->type_SI8  }; }
Value make_Cst_Bool( Bool val ) { return { new Cst( gvm->type_Bool, 1, &val ), 0, gvm->type_Bool }; }

Value make_Cst( Type *type ) { return { new Cst( type, type->content.data.size ), 0, type }; }
