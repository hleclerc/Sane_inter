#pragma once

#include "../System/BoolVec.h"
#include "../Value.h"

/**
*/
class Cst : public Inst {
public:
    template<class T>
    static Value make() {
        return {};
    }

    Cst( int size, void *val = 0, void *kno = 0 );

    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const override;
    virtual void get_bytes  ( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override;
    virtual SI32 size     () const override;

    BoolVec val;
    BoolVec kno;
};

Value make_Cst_SI32( SI32 val );
Value make_Cst_Bool( Bool val );

Value make_Cst( Type *type );
