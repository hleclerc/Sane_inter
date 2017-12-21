#pragma once

#include "../Value.h"

/**
*/
template<class Op>
class UnaOp : public Inst {
public:
    UnaOp( const Value &a ){
        add_child( a );
    }

    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const override {
        os << "pouet";
    }
};

Value make_neg        ( const Value &a );
Value make_not_boolean( const Value &a );
Value make_not_bitwise( const Value &a );
