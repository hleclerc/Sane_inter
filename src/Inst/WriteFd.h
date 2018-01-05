#pragma once

#include "../Value.h"

/**
*/
class WriteFd : public Inst {
public:
    WriteFd( const Vec<Value> &args );

    virtual void write_dot( std::ostream &os, SI32 nout = 0, Type *type = 0, int offset = 0 ) const override;
    virtual int  inp_corr ( int nout ) const;
};

