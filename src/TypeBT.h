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

    virtual void write_cst( std::ostream &os, const PI8 *data, int offset_mod_8 = 0, bool always_add_braces = false ) const override {
        if ( always_add_braces )
            os << "{";

        if ( reverse_endianness )
            TODO;
        if ( offset_mod_8 )
            TODO;

        os << *reinterpret_cast<const T *>( data );

        if ( always_add_braces )
            os << "}";
    }

    virtual bool primitive_number() const {
        return true;
    }

    virtual int is_signed() const {
        return std::is_signed<T>::value;
    }

    virtual int mantissa_len() const {
        if ( std::is_integral<T>::value )
            return 8 * sizeof( T ) - std::is_signed<T>::value;
        TODO;
        return -1;
    }

    virtual int exponent_len() const {
        return -1;
    }
};
