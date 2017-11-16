#pragma once

#include "Inst/Inst.h"
class Type;

/**
*/
class Value {
public:
    Value( const RcPtr<Inst> &inst, Type *type, SI32 offset = 0  ); //
    Value( const Value &value ); //
    Value() {} // void value

    Value      &operator=( const Value &value );

    operator    bool     () const { return inst; }

    RcPtr<Inst> inst;
    Type       *type;
    SI32        offset;
};

