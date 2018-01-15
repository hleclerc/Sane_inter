#pragma once

#include "Clonable.h"

/**
  fd, arg_0, [arg_1, ... arg_{nb_inp}], deps
*/
class WriteFd : public Clonable<WriteFd,Inst> {
public:
    WriteFd( const Vec<Value> &args, int nb_inp );
    WriteFd( AttrClone, const WriteFd * );

    virtual void get_mod_ressources( const std::function<void( Ressource *rs, bool write )> &cb ) const override;
    virtual bool expects_a_reg_at  ( int ninp ) const override;
    virtual bool can_be_inlined    () const override;
    virtual int  nb_outputs        () const override;
    virtual void write_code        ( StreamSep &ss, Codegen &cg ) override;
    virtual void write_dot         ( std::ostream &os ) const override;
    virtual int  inp_corr          ( int nout ) const override;

    int          nb_inp; ///< 1 => write value content. 2 => expects a pointer and a length
};

