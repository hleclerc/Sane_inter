#include "RcString.h"

RcString::RcString( const std::string &str ) : RcString( str.size() ) {
    memcpy( _begin, str.data(), str.size() );
    _begin[ str.size() ] = 0;
}

RcString::RcString( const char *b, const char *e ) : RcString( e - b ) {
    memcpy( _begin, b, e - b );
    _begin[ e - b ] = 0;
}

RcString::RcString( const char *str ) : RcString( strlen( str ) ) {
    memcpy( _begin, str, size() + 1 );
}

RcString::RcString( size_t size ) {
    _begin = reserve( size );
    _end = _begin + size;
}

void RcString::write_to_stream( std::ostream &os ) const {
    os.write( data(), size() );
}

bool RcString::begins_with( const RcString &str ) const {
    return size() >= str.size() && strncmp( data(), str.data(), str.size() ) == 0;
}

void RcString::read_some( void *data, ST size ) {
    memcpy( data, _begin, size );
    _begin += size;
}

char *RcString::reserve( size_t rese ) {
    _content = new ( malloc( sizeof( Content ) - PRC + 1 + rese ) ) Content( rese );
    return _content->data;
}

RcString operator+( const char *a, const RcString &b ) {
    size_t sa = strlen( a );
    RcString res( sa + b.size() );
    memcpy( res.data(), a, sa );
    memcpy( res.data() + sa, b.data(), b.size() );
    res.data()[ sa + b.size() ] = 0;
    return res;
}

RcString operator+( const RcString &a, const RcString &b ) {
    RcString res( a.size() + b.size() );
    memcpy( res.data(), a.data(), a.size() );
    memcpy( res.data() + a.size(), b.data(), b.size() );
    res.data()[ a.size() + b.size() ] = 0;
    return res;
}

RcString operator+( const RcString &a, const char *b ) {
    size_t sb = strlen( b );
    RcString res( a.size() + sb );
    memcpy( res.data(), a.data(), a.size() );
    memcpy( res.data() + a.size(), b, sb );
    res.data()[ a.size() + sb ] = 0;
    return res;
}
