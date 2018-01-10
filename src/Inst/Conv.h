#pragma once

#include "../System/BoolVec.h"
#include "../Value.h"

/**
*/
class Conv : public Inst {
public:
    Conv( const Value &orig, Type *target_type );

    virtual void write_dot( std::ostream &os ) const override;
    virtual void get_bytes( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const override;

    Type        *target_type;
};

Value make_Conv( const Value &orig, Type *target_type );
