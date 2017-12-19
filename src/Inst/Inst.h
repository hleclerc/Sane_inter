#pragma once

#include "../System/RcPtr.h"
class Type;

/**
*/
class Inst : public RcObj {
public:
    virtual ~Inst();

    virtual void write_to_stream( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const;
    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const = 0;
};

