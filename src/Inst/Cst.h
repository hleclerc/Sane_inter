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

    Cst( int size, void *val, void *kno = 0 );

    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const override;

    BoolVec val;
    BoolVec kno;
};

Value make_Cst_SI32( SI32 val );
