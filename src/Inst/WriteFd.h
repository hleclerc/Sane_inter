#pragma once

#include "Clonable.h"
class RessourceMap;

/**
  fd, arg_0, [arg_1, ... arg_{nb_inp}], deps
*/
class WriteFd : public Clonable<WriteFd,Inst> {
public:
    WriteFd( RessourceMap *ressource_map, const Vec<Value> &args );
    WriteFd( AttrClone, const WriteFd * );

    virtual bool expects_a_reg_at  ( int ninp ) const override;
    virtual bool can_be_inlined    () const override;
    virtual int  nb_outputs        () const override;
    virtual void write_code        ( StreamSep &ss, Codegen &cg ) override;
    virtual void write_dot         ( std::ostream &os ) const override;
    virtual int  inp_corr          ( int nout ) const override;

    int          nb_base_args; ///<
};

void make_WriteFd( RessourceMap *ressource_map, const Vec<Value> &args );
