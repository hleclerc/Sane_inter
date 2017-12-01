#pragma once

#include "System/Rc_string.h"
#include "AT.h"

/**
*/
struct String {
    enum {
        short_data_size = 3 * sizeof( void * ) - 1
    };

    #if BYTE_ORDER == LITTLE_ENDIAN
        struct Short {
            unsigned char size; // = 2 * actual_size + 1
            char          data[ short_data_size ];
        };
        struct Long {
            size_t size; // = 2 * actual_size
            size_t rese;
            AT     data;
        };
    #else
        struct Short {
            char          data[ short_data_size ];
            unsigned char size; // = 2 * actual_size + 1
        };
        struct Long {
            AT     data;
            size_t rese;
            size_t size; // = 2 * actual_size
        };
    #endif

    String( const Rc_string &str ) { init( str ); }
    String() {}

    void init( const Rc_string &str ) {
        if ( str.size() <= short_data_size ) {
            Short *content = new ( data ) Short;

            content->size = 2 * str.size() + 1;
            memcpy( content->data, str.ptr(), str.size() );
        } else {
            Long *content = new ( data ) Long;

            content->size = 2 * str.size();
            content->rese = str.size();
            content->data.ptr->content = Variable::Content::alloc( str.size() );
            content->data.ptr->content->cpt_use = 100; //
            memcpy( content->data.ptr->content->data, str.ptr(), str.size() );
        }
    }

    operator Rc_string() const {
        const Short *s = reinterpret_cast<const Short *>( data );
        if ( s->size & 1 )
            return { s->data, s->data + s->size / 2 };
        const Long *l = reinterpret_cast<const Long *>( data );
        return { l->data.ptr->content->data, l->data.ptr->content->data + l->size / 2 };
    }

    const char *ptr() const {
        const Short *s = reinterpret_cast<const Short *>( data );
        if ( s->size & 1 )
            return s->data;
        const Long *l = reinterpret_cast<const Long *>( data );
        return l->data.ptr->content->data;
    }

    //    operator std::string() const {
    //        const Short *s = reinterpret_cast<const Short *>( data );
    //        if ( s->size & 1 )
    //            return { s->data, s->data + s->size / 2 };
    //        const Long *l = reinterpret_cast<const Long *>( data );
    //        return { l->data.ptr->content->data, l->data.ptr->content->data + l->size / 2 };
    //    }

    void write_to_stream( std::ostream &os ) const { os << Rc_string( *this ); }

    char data[ sizeof( Long ) ];
};
