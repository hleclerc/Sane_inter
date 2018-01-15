#include "BoolVec.h"
#include "Memcpy.h"

BoolVec::BoolVec( Reference, void *data, size_t size ) : data( (PI8 *)data ), size( size ), rese( 0 ) {
}

BoolVec::BoolVec( size_t size, bool val ) : size( size ), rese( size ) {
    size_t bs = ( size + 7 ) / 8;
    if( size )
        data = (PI8 *)malloc( bs );

    if ( val )
        for( size_t i = 0; i < bs; ++i )
            data[ i ] = 255;
    else
        for( size_t i = 0; i < bs; ++i )
            data[ i ] = 0;
}

BoolVec::BoolVec( size_t size ) : size( size ), rese( size ) {
    size_t bs = ( size + 7 ) / 8;
    if ( size )
        data = (PI8 *)malloc( bs );
}

BoolVec::~BoolVec() {
    if ( rese )
        free( data );
}

void BoolVec::write_to_stream( std::ostream &os ) const {
    for( size_t i = 0; i < size; ++i )
        os << get( i );
}

bool BoolVec::operator==( const BoolVec &that ) const {
    return size == that.size && memcmp_bit( data, 0, that.data, 0, size ) == 0;
}

bool BoolVec::operator!=( const BoolVec &that ) const {
    return ! operator ==( that );
}

bool BoolVec::all_true() const {
    if ( size ) {
        size_t bs = size / 8;
        if ( bs )
            for( size_t i = 0; i < bs; ++i )
                if ( data[ i ] != 255 )
                    return false;
        return size % 8 == 0 || PI8( data[ bs ] | ( 255 << size % 8 ) ) == 255;
    }
    return true;
}

bool BoolVec::all_false() const {
    if ( size ) {
        size_t bs = size / 8;
        if ( bs )
            for( size_t i = 0; i < bs; ++i )
                if ( data[ i ] )
                    return false;
        return size % 8 == 0 || PI8( data[ bs ] & ~ ( 255 << size % 8 ) ) == 0;
    }
    return true;
}

bool BoolVec::all_true( size_t beg, size_t end ) const {
    if ( beg % 8 )
        TODO;

    if ( size_t size = end - beg ) {
        size_t bs = size / 8;
        beg /= 8;
        if ( bs )
            for( size_t i = 0; i < bs; ++i )
                if ( data[ beg + i ] != 255 )
                    return false;
        return size % 8 == 0 || PI8( data[ beg + bs ] | ( 255 << size % 8 ) ) == 255;
    }
    return true;
}

bool BoolVec::all_false( size_t beg, size_t end ) const {
    if ( beg % 8 )
        TODO;

    if ( size_t size = end - beg ) {
        size_t bs = size / 8;
        beg /= 8;
        if ( bs )
            for( size_t i = 0; i < bs; ++i )
                if ( data[ beg + i ] )
                    return false;
        return size % 8 == 0 || PI8( data[ beg + bs ] & ~ ( 255 << size % 8 ) ) == 0;
    }
    return true;
}

void BoolVec::set( size_t index, bool val ) {
    if ( val )
        data[ index / 8 ] |= ( 1 << index % 8 );
    else
        data[ index / 8 ] &= ~ ( 1 << index % 8 );
}

void BoolVec::set( bool val ) {
    size_t bs = ( size + 7 ) / 8;
    if ( val )
        for( size_t i = 0; i < bs; ++i )
            data[ i ] = 255;
    else
        for( size_t i = 0; i < bs; ++i )
            data[ i ] = 0;

}

bool BoolVec::get( size_t index ) const {
    return data[ index / 8 ] & ( 1 << index % 8 );
}

void BoolVec::set( size_t beg, size_t end, bool val ) {
    PI8 *dst = data;
    if ( size_t o = beg / 8 ) {
        dst += o;
        beg %= 8;
    }

    if ( val ) {
        if ( beg ) {
            TODO;
        }
        size_t len = end - beg;
        for( ; len >= 8; len -= 8, ++dst )
            *(PI8 *)dst = 0xFF;
        if ( len )
            *(PI8 *)dst |= ~( 0xFF << len );
    } else {
        if ( beg ) {
            TODO;
        }
        size_t len = end - beg;
        for( ; len >= 8; len -= 8, ++dst )
            *(PI8 *)dst = 0x00;
        if ( len )
            *(PI8 *)dst &= 0xFF << len;
    }
}
