#pragma once

#include "Ref.h"

/**
*/
template<class T>
class KnownRef : public Ref {
public:
    template<class ...Args>
    KnownRef( Args &&...args ) : data( std::forward<Args>( args )... ) {
    }

    virtual void write_to_stream( std::ostream &os ) const {
        os << data;
    }

    virtual void *rcast() {
        return &data;
    }

    virtual Value get() const override {
        TODO;
        return {};
    }

    virtual void set( const Value &val, SI32 offset ) override {
        TODO;
    }

    T data;
};
