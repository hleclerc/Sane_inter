#pragma once

#include "Clonable.h"

/**
  fd, arg_0, [arg_1, ... arg_{nb_inp}], deps
*/
class ReadFd : public Clonable<ReadFd,Inst> {
public:
    ReadFd( const Value &fd, const Value &ptr, const Value &len );
    ReadFd( AttrClone, const ReadFd * );

    virtual void get_mod_ressources( const std::function<void( const Value &fd, int mod_type )> &cb ) const override;
    //    virtual bool expects_a_reg_at  ( int ninp ) const override;
    //    virtual bool can_be_inlined    () const override;
    virtual int  nb_outputs        () const override;
    //    virtual void write_code        ( StreamSep &ss, Codegen &cg ) override;
    virtual void write_dot         ( std::ostream &os ) const override;
    //    virtual int  inp_corr          ( int nout ) const;
};

