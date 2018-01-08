#pragma once

#include "../Value.h"

/**
*/
class WriteFd : public Inst {
public:
    WriteFd( const Vec<Value> &args );

    virtual void write_dot( std::ostream &os ) const override;
    virtual int  inp_corr ( int nout ) const;
};

