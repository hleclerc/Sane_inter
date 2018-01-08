#pragma once

#include "../System/Memcpy.h"
#include "../Value.h"

/**
*/
template<class T>
class KnownVal : public Inst {
public:
    template<class ...Args>
    KnownVal( Type *type, Args&& ...args ) : type( type ), data( std::forward<Args>( args )... ) {}

    virtual void write_dot( std::ostream &os ) const override {

    }
    virtual void get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override {
        if ( beg_src >= 8 * sizeof( data ) )
            return;
        if ( beg_src + len > 8 * sizeof( data ) )
            len = 8 * sizeof( data ) - beg_src;
        memcpy_bit( dst, beg_dst, &data, beg_src, len, msk );
        memset_bit( msk, beg_dst, false, len );
    }

    virtual void *rcast( SI32 nout, Type *type, SI32 offset ) override {
        return &data;
    }

    Type *type;
    T     data;
};

template<class T,class ...Args>
Value make_KnownVal( Type *type, Args&& ...args ) {
    return { new KnownVal<T>( type, std::forward<Args>( args )... ), 0, type };
}

#define MAKE_KV( T, ... ) make_KnownVal<T>( gvm->type_##T, ##__VA_ARGS__ )
