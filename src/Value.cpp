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

