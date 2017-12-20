#include "System/BoolVec.h"
#include "Value.h"

Value::Value( const RcPtr<Inst> &inst, SI32 nout, Type *type, SI32 offset ) : inst( inst ), type( type ), nout( nout ), offset( offset ) {
}

Value::Value( const Value &value ) : inst( value.inst ), type( value.type ), nout( value.nout ), offset( value.offset ) {
}

Value &Value::operator=( const Value &value ) {
    inst   = value.inst;
    type   = value.type;
    nout   = value.nout;
    offset = value.offset;
    return *this;
}

void Value::write_to_stream( std::ostream &os ) const {
    if ( inst )
        inst->write_to_stream( os, nout, type, offset );
    else
        os << "NULL";
}

bool Value::get_bytes( void *dst, PI32 beg_dst, PI32 beg_src, PI32 len ) const {
    BoolVec msk( len, true );
    get_bytes( dst, beg_dst, beg_src, len, msk.data );
    return msk.all_false();
}

void Value::get_bytes( void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const {
    return inst->get_bytes( nout, dst, beg_dst, beg_src + offset, len, msk );
}

