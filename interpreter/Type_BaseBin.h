#pragma once

#include "Type.h"

/**
*/
template<class T>
class Type_BaseBin : public Type {
public:
    Type_BaseBin( const Rc_string &name ) : Type( name, 8 * sizeof( T ) ) {}

    virtual void write_to_stream( std::ostream &os, const char *content ) const override { os << *reinterpret_cast<const T *>( content ); }
    virtual void bin_ctor       ( Scope *scope, const Variable &self ) override { new ( self.ptr() ) T; }
    virtual void destroy        ( Scope *scope, const Variable &self, bool use_virtual ) override { reinterpret_cast<T *>( self.ptr() )->~T(); }
};

