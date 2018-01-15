#pragma once

#include "Clonable.h"

/**
  fd, arg_0, [arg_1, ... arg_{nb_inp}], deps
*/
class ReadFdAt : public Clonable<ReadFdAt,Inst> {
public:
    ReadFdAt( const Value &fd, const Value &ptr, const Value &off, const Value &len );
    ReadFdAt( AttrClone, const ReadFdAt * );

    virtual void get_mod_ressources( const std::function<void( Ressource *rs, bool write )> &cb ) const override;
    virtual int  nb_outputs        () const override;
//    virtual bool expects_a_reg_at  ( int ninp ) const override;
//    virtual bool can_be_inlined    () const override;
//    virtual int  nb_outputs        () const override;
//    virtual void write_code        ( StreamSep &ss, Codegen &cg ) override;
    virtual void write_dot         ( std::ostream &os ) const override;
//    virtual int  inp_corr          ( int nout ) const;
};

