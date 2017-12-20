#include "Memcpy.h"
#include "Assert.h"
#include <string.h>

void memcpy_bit( void *dst, size_t off_dst, const void *src, size_t off_src, size_t len, const void *msk ) {
    // remove byte offsets (to have off_xxx < 8 )
    if ( size_t o = off_dst / 8 ) return memcpy_bit( (PI8 *)dst + o, off_dst % 8, src, off_src, len, msk );
    if ( size_t o = off_src / 8 ) return memcpy_bit( dst, off_dst, (PI8 *)src + o, off_src % 8, len, msk );

    // bit offsets
    if ( off_src % 8 || off_src % 8 ) {
        TODO;
        return;
    }

    // TODO: optimize for large sizes
    if ( msk ) {
        for( size_t i = 0; i < ( len + 7 ) / 8; ++i )
            reinterpret_cast<char *>( dst )[ i ] =
                ( reinterpret_cast<const char *>( dst )[ i ] & ~ reinterpret_cast<const char *>( msk )[ i ] ) |
                ( reinterpret_cast<const char *>( src )[ i ] &   reinterpret_cast<const char *>( msk )[ i ] );
        return;
    }

    // else, call the usual memcpy
    memcpy( dst, src, ( len + 7 ) / 8 );
}

void memcpy_bit( void *dst, const void *src, size_t len ) {
    return memcpy_bit( dst, 0, src, 0, len );
}

void memset_bit( void *dst, size_t off_dst, bool val, size_t len ) {
    if ( size_t o = off_dst / 8 ) {
        dst = (PI8 *)dst + o;
        off_dst %= 8;
    }
    if ( val ) {
        if ( off_dst ) {
            TODO;
        }
        for( ; len >= 8; len -= 8, dst = (PI8 *)dst + 1 )
            *(PI8 *)dst = 0xFF;
        if ( len )
            *(PI8 *)dst |= ~( 0xFF << len );
    } else {
        if ( off_dst ) {
            TODO;
        }
        for( ; len >= 8; len -= 8, dst = (PI8 *)dst + 1 )
            *(PI8 *)dst = 0x00;
        if ( len )
            *(PI8 *)dst &= 0xFF << len;
    }
}

int memcmp_bit( const void *dst, size_t off_dst, const void *src, size_t off_src, size_t len ) {
    if ( off_src % 8 or off_src % 8 ) {
        TODO;
        return 0;
    }

    return memcmp( (const char *)dst + off_dst / 8, (const char *)src + off_src / 8, ( len + 7 ) / 8 );
}
