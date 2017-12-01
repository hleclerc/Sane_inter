#pragma once

#include "Stream.h"

/**
  Reference on a Boolean, somewhere in the memory
*/
class BoolRef {
public:
    BoolRef( char *data, int offset ) : data( data ), offset( offset ) {
    }

    operator bool() const {
        #if BYTE_ORDER == LITTLE_ENDIAN
        return data[ offset / 8 ] & ( 1 << ( offset % 8 ) );
        #else
        return data[ offset / 8 ] & ( 128 >> ( offset % 8 ) );
        #endif
    }

    void operator=( bool val ) {
        #if BYTE_ORDER == LITTLE_ENDIAN
        if ( val )
            data[ offset / 8 ] |=  ( 1 << ( offset % 8 ) );
        else
            data[ offset / 8 ] &= ~( 1 << ( offset % 8 ) );
        #else
        if ( val )
            data[ offset / 8 ] |=  ( 128 >> ( offset % 8 ) );
        else
            data[ offset / 8 ] &= ~( 128 >> ( offset % 8 ) );
        #endif
    }

    void operator=( const BoolRef &br ) {
        return operator=( bool( br ) );
    }

    void write_to_stream( std::ostream &os ) const {
        os << operator bool();
    }

    char *data;
    int   offset;
};

