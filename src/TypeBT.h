#pragma once

#include "Type.h"
#include "gvm.h"

/**
*/
template<class T,bool reverse_endianness>
class TypeBT : Type {
public:
    TypeBT( const LString &name ) : Type( name ) {
    }

    virtual void write_cst( std::ostream &os, const PI8 *data, int offset_mod_8 = 0 ) const override {
        if ( reverse_endianness )
            TODO;
        if ( offset_mod_8 )
            TODO;
        os << *reinterpret_cast<const T *>( data );
    }

    virtual bool primitive_number() const {
        return true;
    }
};
