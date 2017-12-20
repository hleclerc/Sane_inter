#pragma once

#include "Inst/Inst.h"
class BoolVec;
class Type;

/**
*/
class Value {
public:
    Value( const RcPtr<Inst> &inst, SI32 nout, Type *type, SI32 offset = 0  ); //
    Value( const Value &value ); //
    Value() {} // void value

    Value      &operator=      ( const Value &value );

    operator    bool           () const { return inst; }

    void        write_to_stream( std::ostream &os ) const;

    bool        get_bytes        ( void *dst, PI32 beg_dst, PI32 beg_src, PI32 len ) const;
    void        get_bytes        ( void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const;

    RcPtr<Inst> inst;
    Type       *type;
    SI32        nout;
    SI32        offset;
};

