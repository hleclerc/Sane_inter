#include "Rc_string.h"

Rc_string::Rc_string( const std::string &str ) : Rc_string( str.size() ) {
    memcpy( _begin, str.data(), str.size() );
    _begin[ str.size() ] = 0;
}

Rc_string::Rc_string( const char *b, const char *e ) : Rc_string( e - b ) {
    memcpy( _begin, b, e - b );
    _begin[ e - b ] = 0;
}

Rc_string::Rc_string( const char *str ) : Rc_string( strlen( str ) ) {
    memcpy( _begin, str, size() + 1 );
}

Rc_string::Rc_string( size_t size ) {
    _begin = reserve( size );
    _end = _begin + size;
}

void Rc_string::write_to_stream( std::ostream &os ) const {
    os.write( data(), size() );
}

bool Rc_string::begins_with( const Rc_string &str ) const {
    return size() >= str.size() && strncmp( data(), str.data(), str.size() ) == 0;
}

void Rc_string::read_some( void *data, ST size ) {
    memcpy( data, _begin, size );
    _begin += size;
}

char *Rc_string::reserve( size_t rese ) {
    _content = new ( malloc( sizeof( Content ) - PRC + 1 + rese ) ) Content( rese );
    return _content->data;
}

Rc_string operator+( const char *a, const Rc_string &b ) {
    size_t sa = strlen( a );
    Rc_string res( sa + b.size() );
    memcpy( res.data(), a, sa );
    memcpy( res.data() + sa, b.data(), b.size() );
    res.data()[ sa + b.size() ] = 0;
    return res;
}

Rc_string operator+( const Rc_string &a, const Rc_string &b ) {
    Rc_string res( a.size() + b.size() );
    memcpy( res.data(), a.data(), a.size() );
    memcpy( res.data() + a.size(), b.data(), b.size() );
    res.data()[ a.size() + b.size() ] = 0;
    return res;
}

Rc_string operator+( const Rc_string &a, const char *b ) {
    size_t sb = strlen( b );
    Rc_string res( a.size() + sb );
    memcpy( res.data(), a.data(), a.size() );
    memcpy( res.data() + a.size(), b, sb );
    res.data()[ a.size() + sb ] = 0;
    return res;
}
