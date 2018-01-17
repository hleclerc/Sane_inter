#include "RessourceInst.h"
#include "../gvm.h"

RessourceInst::RessourceInst( Ressource *rs ) : rs( rs ) {
}

RessourceInst::RessourceInst( AttrClone, const RessourceInst *orig ) : rs( orig->rs ) {
}

int RessourceInst::nb_outputs() const {
    return 1;
}

void RessourceInst::write_dot( std::ostream &os ) const {
    if ( rs )
        os << *rs;
    else
        os << "NULL RS";
}

Type *RessourceInst::out_type( int nout ) const {
    return gvm->type_Ressource;
}

void RessourceInst::write_code( StreamSep &ss, Codegen &cg ) {
}

Value make_RessourceInst( Ressource *rs ) {
    return { new RessourceInst( rs ), 0, gvm->type_Ressource };
}
