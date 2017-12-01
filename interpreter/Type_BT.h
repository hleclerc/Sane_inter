#pragma once

#include "System/BoolRef.h"
#include "Type.h"

/**
*/
template<class T>
class Type_BT : public Type {
public:
    Type_BT( const char *name, SI32 size = 8 * sizeof( T ) ) : Type( name, size, std::min( size, 64 ) ) {}

    virtual void destroy( Scope *scope, const Variable &self, bool use_virtual ) override {
    }

    virtual bool base_arythmetic_type() const override {
        return true;
    }

    virtual void write_to_stream( std::ostream &os, const char *content ) const {
        if ( size == 8 )
            os << (int)*reinterpret_cast<const T *>( content );
        else
            os << *reinterpret_cast<const T *>( content );
    }

    virtual bool is_false( Scope *scope, const Variable &self ) {
        return ! *reinterpret_cast<const T *>( self.ptr() );
    }

    virtual bool is_true( Scope *scope, const Variable &self ) {
        return *reinterpret_cast<const T *>( self.ptr() );
    }
};

