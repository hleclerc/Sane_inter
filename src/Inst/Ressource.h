#pragma once

#include "Clonable.h"

/**
*/
class Ressource : public Clonable<Ressource> {
public:
    Ressource();
    Ressource( AttrClone, const Ressource *orig );

    virtual int   nb_outputs() const override;
    virtual void  write_code( StreamSep &ss, Codegen &cg ) override;
    virtual void  write_dot ( std::ostream &os ) const override;
    virtual Type *out_type  ( int nout ) const override;
};

Value make_Ressource();
