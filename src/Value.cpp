#include "Value.h"

Value::Value( const RcPtr<Inst> &inst, Type *type, SI32 offset ) : inst( inst ), type( type ), offset( offset ) {
}

Value::Value( const Value &value ) {
}

Value &Value::operator=( const Value &value ) {
    inst   = value.inst;
    type   = value.type;
    offset = value.offset;
    return *this;
}

void Value::write_to_stream( std::ostream &os ) const {
    if ( inst )
        inst->write_to_stream( os, type, offset );
    else
        os << "NULL";
}

