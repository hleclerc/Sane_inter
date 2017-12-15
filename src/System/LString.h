#pragma once

#include "RcString.h"

/**
*/
struct LString {
    enum {
        short_data_size = 3 * sizeof( void * ) - 1
    };

    #if BYTE_ORDER == LITTLE_ENDIAN
        struct Short {
            unsigned char size_m2_p1; // = 2 * actual_size + 1
            char          data[ short_data_size ];
        };
        struct Long {
            PT     size_m2; // = 2 * actual_size
            PT     rese;
            char  *data;
        };
    #else
        struct Short {
            char          data[ short_data_size ];
            unsigned char size_m2_p1; // = 2 * actual_size + 1
        };
        struct Long {
            AT     data;
            size_t rese;
            size_t size_m2_p1; // = 2 * actual_size
        };
    #endif

    LString( const RcString &str ) { init_with_str( str.data(), str.size() ); }
    LString( const LString  &str ) { init_with_str( str.data(), str.size() ); }
    LString( const String   &str ) { init_with_str( str.data(), str.size() ); }
    LString( const char     *str ) { init_with_str( str, strlen( str ) ); }

    LString() { Short *content = new ( _data ) Short; content->size_m2_p1 = 1; }

    ~LString();

    operator    RcString  () const;

    LString     operator= ( const RcString &str ) { return reassign_with_str( str.data(), str.size() ); }
    LString     operator= ( const LString  &str ) { return reassign_with_str( str.data(), str.size() ); }
    LString     operator= ( const String   &str ) { return reassign_with_str( str.data(), str.size() ); }
    LString     operator= ( const char     *str ) { return reassign_with_str( str, strlen( str ) ); }

    bool        operator==( const RcString &str ) const { return equal_with_str( str.data(), str.size() ); }
    bool        operator==( const LString  &str ) const { return equal_with_str( str.data(), str.size() ); }
    bool        operator==( const String   &str ) const { return equal_with_str( str.data(), str.size() ); }
    bool        operator==( const char     *str ) const { return equal_with_str( str, strlen( str ) ); }

    bool        operator!=( const RcString &str ) const { return ! equal_with_str( str.data(), str.size() ); }
    bool        operator!=( const LString  &str ) const { return ! equal_with_str( str.data(), str.size() ); }
    bool        operator!=( const String   &str ) const { return ! equal_with_str( str.data(), str.size() ); }
    bool        operator!=( const char     *str ) const { return ! equal_with_str( str, strlen( str ) ); }

    const char *data      () const;
    char       *data      ();
    PT          size      () const;
    PT          rese      () const;
    bool        empty     () const;
    void        resize    ( PT new_size );
    void        reserve   ( PT new_rese );

    //    operator std::string() const {
    //        const Short *s = reinterpret_cast<const Short *>( data );
    //        if ( s->size & 1 )
    //            return { s->data, s->data + s->size / 2 };
    //        const Long *l = reinterpret_cast<const Long *>( data );
    //        return { l->data.ptr->content->data, l->data.ptr->content->data + l->size / 2 };
    //    }

    void write_to_stream( std::ostream &os ) const {
        os << RcString( *this );
    }

private:
    void     init_with_str    ( const char *str_data, PT str_size );
    bool     equal_with_str   ( const char *str_data, PT str_size ) const;
    LString &reassign_with_str( const char *str_data, PT str_size );

    char     _data[ sizeof( Long ) ];
};

inline LString::~LString() {
    Short *s = reinterpret_cast<Short *>( _data );
    Long *l = reinterpret_cast<Long *>( _data );
    if ( ( s->size_m2_p1 & 1 ) == 0 && l->rese )
        free( l->data );
}

inline const char *LString::data() const {
    const Short *s = reinterpret_cast<const Short *>( _data );
    if ( s->size_m2_p1 & 1 )
        return s->data;
    const Long *l = reinterpret_cast<const Long *>( _data );
    return l->data;
}

inline char *LString::data() {
    Short *s = reinterpret_cast<Short *>( _data );
    if ( s->size_m2_p1 & 1 )
        return s->data;
    Long *l = reinterpret_cast<Long *>( _data );
    return l->data;
}

inline PT LString::size() const {
    const Short *s = reinterpret_cast<const Short *>( _data );
    if ( s->size_m2_p1 & 1 )
        return s->size_m2_p1 / 2;
    const Long *l = reinterpret_cast<const Long *>( _data );
    return l->size_m2 / 2;
}

inline PT LString::rese() const {
    const Short *s = reinterpret_cast<const Short *>( _data );
    if ( s->size_m2_p1 & 1 )
        return short_data_size;
    const Long *l = reinterpret_cast<const Long *>( _data );
    return l->rese;
}

inline bool LString::empty() const {
    return size() == 0;
}

inline bool LString::equal_with_str( const char *str_data, PT str_size ) const {
    return str_size == size() && memcmp( str_data, data(), str_size ) == 0;
}

