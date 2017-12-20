#pragma once

#include <byteswap.h>
#include "N.h"

template<class T> inline T byte_swaped( const T &val, N<1> ) { return val; }
template<class T> inline T byte_swaped( const T &val, N<2> ) { return bswap_16( val ); }
template<class T> inline T byte_swaped( const T &val, N<4> ) { return bswap_32( val ); }
template<class T> inline T byte_swaped( const T &val, N<8> ) { return bswap_64( val ); }

template<class T> inline T byte_swaped( const T &val ) { return byte_swaped( val, N<sizeof(val)>() ); }

