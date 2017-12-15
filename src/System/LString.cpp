#include "LString.h"

LString::operator RcString() const {
    const Short *s = reinterpret_cast<const Short *>( _data );
    if ( s->size_m2_p1 & 1 )
        return { s->data, s->data + s->size_m2_p1 / 2 };
    const Long *l = reinterpret_cast<const Long *>( _data );
    return { l->data, l->data + l->size_m2 / 2 };
}

void LString::resize(PT new_size) {
    reserve( new_size );

    Short *s = reinterpret_cast<Short *>( _data );
    if ( s->size_m2_p1 & 1 )
        s->size_m2_p1 = 2 * new_size + 1;
    else
        reinterpret_cast<Long *>( _data )->size_m2 = 2 * new_size;
}

void LString::reserve(PT new_rese) {
    const Short *s = reinterpret_cast<const Short *>( _data );
    if ( s->size_m2_p1 & 1 ) {
        if ( new_rese > short_data_size ) {
            char *new_data = (char *)malloc( new_rese );
            memcpy( new_data, s->data, s->size_m2_p1 / 2 );
            PT old_size = s->size_m2_p1 / 2;

            // overwrite this->data
            Long *l = reinterpret_cast<Long *>( _data );
            l->data = new_data;
            l->rese = new_rese;
            l->size_m2 = 2 * old_size;
        }
    } else {
        Long *l = reinterpret_cast<Long *>( _data );
        if ( new_rese > l->rese ) {
            char *new_data = (char *)malloc( new_rese );
            memcpy( new_data, l->data, l->size_m2 / 2 );
            if ( l->rese )
                free( l->data );
            l->data = new_data;
            l->rese = new_rese;
        }
    }
}

void LString::init_with_str( const char *str_data, PT str_size ) {
    if ( str_size <= short_data_size ) {
        Short *content = new ( _data ) Short;

        content->size_m2_p1 = 2 * str_size + 1;
        memcpy( content->data, str_data, str_size );
    } else {
        Long *content = new ( _data ) Long;

        content->size_m2 = 2 * str_size;
        content->rese = str_size;
        content->data = (char *)malloc( str_size );
        memcpy( content->data, str_data, str_size );
    }
}

LString &LString::reassign_with_str( const char *str_data, PT str_size ) {
    resize( str_size );
    memcpy( data(), str_data, str_size );
    return *this;
}
