#pragma once

#include "Clonable.h"

/**
*/
class RessourceInst : public Clonable<RessourceInst> {
public:
    RessourceInst( Ressource *rs );
    RessourceInst( AttrClone, const RessourceInst *orig );

    virtual int   nb_outputs() const override;
    virtual void  write_code( StreamSep &ss, Codegen &cg ) override;
    virtual void  write_dot ( std::ostream &os ) const override;
    virtual Type *out_type  ( int nout ) const override;

    Ressource    *rs;
};

Value make_RessourceInst( Ressource *rs );