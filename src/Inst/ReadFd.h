#pragma once

#include "Clonable.h"
class RessourceMap;

/**
  inputs:
    fd,
    old val,
    len,
    file cursor/content ressources,

  outputs:
    new val
    file cursor/content ressources,
*/
class ReadFd : public Clonable<ReadFd,Inst> {
public:
    ReadFd( RessourceMap *ressource_map, const Value &fd, const Value &val, const Value &len );
    ReadFd( AttrClone, const ReadFd * );

    virtual int  nb_outputs         () const override;
    virtual void write_dot          ( std::ostream &os ) const override;
    virtual int  inp_corr           ( int nout ) const override;

    //    virtual bool expects_a_reg_at  ( int ninp ) const override;
    //    virtual bool can_be_inlined    () const override;
    //    virtual void write_code        ( StreamSep &ss, Codegen &cg ) override;
    //    virtual int  inp_corr          ( int nout ) const;

    int          beg_cursors;
};

Value make_ReadFd( RessourceMap *ressource_map, const Value &fd, const Value &val, const Value &len );
