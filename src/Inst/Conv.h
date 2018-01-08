#pragma once

#include "../System/BoolVec.h"
#include "../Value.h"

/**
*/
class Conv : public Inst {
public:
    Conv( const Value &orig, Type *target_type );

    virtual void write_dot( std::ostream &os ) const override;
    virtual void get_bytes( SI32 nout, void *dst, PI32 beg_dst, PI32 beg_src, PI32 len, void *msk ) const override;

    Type        *target_type;
};

Value make_Conv( const Value &orig, Type *target_type );
