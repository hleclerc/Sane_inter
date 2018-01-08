#pragma once

#include "Clonable.h"

/**
*/
class WriteFd : public Clonable<WriteFd,Inst> {
public:
    WriteFd( const Vec<Value> &args );
    WriteFd( AttrClone, const WriteFd * );

    virtual void write_code   ( StreamSep &ss, Codegen &cg ) override;
    virtual void write_dot    ( std::ostream &os ) const override;
    virtual int  inp_corr     ( int nout ) const;
};

