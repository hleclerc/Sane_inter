#include "System/BoolVec.h"
#include "Value.h"
#include "Type.h"

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

bool Value::operator<( const Value &that ) const {
    return std::tie( inst, type, nout, offset ) < std::tie( that.inst, that.type, that.nout, that.offset );
}

bool Value::operator==( const Value &that ) const {
    return std::tie( inst, type, nout, offset ) == std::tie( that.inst, that.type, that.nout, that.offset );
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

void *Value::rcast() {
    return inst->rcast( nout, type, offset );
}

bool Value::is_equal( const Value &that ) const {
    if ( type == that.type ) {
        BoolVec bv_this( this->type->size() );
        BoolVec bv_that( that. type->size() );
        if ( this->get_bytes( bv_this.data, 0, 0, this->type->size() ) &&
             that. get_bytes( bv_that.data, 0, 0, that. type->size() ) )
            return bv_this == bv_that;
        return false;
    }

    TODO;
    return false;
}

bool Value::is_not_equal( const Value &that ) const {
    if ( type == that.type ) {
        BoolVec bv_this( this->type->size() );
        BoolVec bv_that( that. type->size() );
        if ( this->get_bytes( bv_this.data, 0, 0, this->type->size() ) &&
             that. get_bytes( bv_that.data, 0, 0, that. type->size() ) )
            return bv_this != bv_that;
        return false;
    }

    TODO;
    return false;
}

