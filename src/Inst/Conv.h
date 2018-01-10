#pragma once

#include "../System/BoolVec.h"
#include "Clonable.h"

/**
*/
class Conv : public Clonable<Conv> {
public:
    Conv( const Value &orig, Type *target_type );
    Conv( AttrClone, const Conv *orig );

    virtual int   nb_outputs() const override;
    virtual void  write_dot ( std::ostream &os ) const override;
    virtual Type *out_type  ( int nout ) const override;

    // virtual void get_bytes( int nout, void *dst, int beg_dst, int beg_src, int len, void *msk ) const override;

    Type        *target_type;
};

Value make_Conv( const Value &orig, Type *target_type );
