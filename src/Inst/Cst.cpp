#include "../System/byte_swap.h"
#include "../System/Memcpy.h"
#include "../Codegen/Codegen.h"
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

Cst::Cst( AttrClone, const Cst *cst ) : type( cst->type ), val( cst->val ), kno( cst->kno ) {
}

void Cst::write_inline_code( StreamPrio &ss, Codegen &cg ) {
    if ( ! kno.all_false() )
        type->write_cst( ss.stream, val.data, 0, true );
    else
        ss << "{}";
}

void Cst::write_to_stream( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    if ( type )
        type->write_cst( os, val.data + offset / 8, offset % 8 );
    else
        this->type->write_cst( os, val.data, 0 );
}

void Cst::write_dot( std::ostream &os ) const {
    type->write_cst( os, val.data, 0 );
}

void Cst::get_bytes( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const {
    if ( PI32( beg_src ) >= val.size )
        return;
    if ( PI32( beg_src + len ) > val.size )
        len = val.size - beg_src;
    memcpy_bit( dst, beg_dst, val.data, beg_src, len, msk );
    memset_bit( msk, beg_dst, false, len );
}

Type *Cst::out_type( int nout ) const {
    return type;
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

