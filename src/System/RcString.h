#pragma once

#include <string.h>
#include "xxhash.h"
#include "RcPtr.h"

/**
*/
class RcString {
public:
    enum { PRC = 8 };

    RcString( RcString &rc, char *b, char *e ) : _content( rc._content ), _begin( b ), _end( e ) { ASSERT_IF_DEBUG( _end == 0 || _end <= _content->data + _content->rese ); }
    RcString( const char *b, const char *e );
    RcString( const std::string &str );
    RcString( const char *str );
    RcString( size_t size );
    RcString() { _begin = 0; _end = 0; }

    RcString &operator=( const RcString &that ) { _content = that._content; _begin = that._begin; _end = that._end; return *this; }

    operator          std::string    () const { return { _begin, _end }; }
    explicit operator bool           () const { return size(); }
    bool              operator<      ( const RcString &that ) const { return size() == that.size() ? strncmp( _begin, that._begin, size() ) < 0 : size() < that.size(); }
    bool              operator==     ( const RcString &that ) const { return size() == that.size() && strncmp( _begin, that._begin, size() ) == 0; }
    bool              operator!=     ( const RcString &that ) const { return size() != that.size() || strncmp( _begin, that._begin, size() ) != 0; }

    void              write_to_stream( std::ostream &os ) const;
    bool              ack_read_byte  () { return _begin < _end ? true : ack_error(); } ///< return true if ok to read a byte. Else, set end to 0 (to signal an error) and return false.
    bool              ack_read_some  ( ST len ) { return _begin + len <= _end ? true : ack_error(); } ///< return true if ok to `len` bytes. Else, set end to 0 (to signal an error) and return false.
    bool              begins_with    ( const RcString &str ) const;
    bool              ack_error      () { _begin = 0; _end = 0; return false; } ///< set error flag to true, and return false
    void              read_some      ( void *data, ST size );
    void              skip_some      ( ST size ) { _begin += size; ASSERT_IF_DEBUG( _begin <= _end ); }
    PI8               read_byte      () { return *( _begin++ ); }
    void              clear          () { _content.clear(); _begin = 0; _end = 0; }
    bool              empty          () const { return _end == _begin; }
    bool              error          () const { return _end == 0; } ///< works after at least a first read (and before free or clear)
    char             *begin          () const { return _begin; }
    char             *data           () const { return _begin; }
    size_t            size           () const { return _end - _begin; }
    char             *ptr            () const { return _begin; }
    char             *end            () const { ASSERT_IF_DEBUG( _content == nullptr || _end <= _content->data + _content->rese ); return _end; }


protected:
    struct Content : public RcObj {
        Content( size_t rese ) : rese( rese ) {}

        size_t rese;
        char   data[ PRC ]; ///< actual reservation: rese + 1 (for the null char)
    };

    char             *reserve( size_t rese );

    RcPtr<Content,1> _content;
    char             *_begin;
    char             *_end;
};

RcString operator+( const char      *a, const RcString &b );
RcString operator+( const RcString &a, const RcString &b );
RcString operator+( const RcString &a, const char      *b );

namespace std {
    template<> struct hash<RcString> {
        typedef RcString argument_type;
        typedef std::size_t result_type;
        result_type operator()( argument_type const &s ) const {
            //            result_type hash = 5381;
            //            for( const char *v = ; ++v )
            //                hash = ( ( hash << 5 ) + hash ) + *v;
            return xxh::xxhash<8*sizeof(void*)>( (const void *)s.begin(), s.size() );
        }
    };
}
