#pragma once

#include "Clonable.h"

/**
*/
class Rand : public Clonable<Rand,Inst> {
public:
    Rand( Type *type );
    Rand( AttrClone, const Rand *inst );

    virtual void  write_inline_code( StreamPrio &ss, Codegen &cg, int nout, int flags ) override;
    virtual int   nb_outputs       () const override;
    virtual void  write_dot        ( std::ostream &os ) const override;
    virtual Type *out_type         ( int nout ) const override;

    Type        *type;
};

Value make_Rand( Type *type );
