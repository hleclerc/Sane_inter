#pragma once

#include "../System/RcPtr.h"
class BoolVec;
class Value;
class Type;

/**
*/
class Inst : public RcObj {
public:
    virtual ~Inst();

    void         add_child      ( const Value &ch );

    virtual void write_to_stream( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const;
    virtual void write_dot      ( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const = 0;
    virtual void get_bytes        ( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const;
    virtual SI32 size           () const;

    Vec<Value>   children;
};

