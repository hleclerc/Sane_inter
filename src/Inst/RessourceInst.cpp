#include "RessourceInst.h"
#include "../gvm.h"

RessourceInst::RessourceInst() {
}

RessourceInst::RessourceInst( AttrClone, const Ressource *orig ) {
}

int RessourceInst::nb_outputs() const {
    return 1;
}

void RessourceInst::write_dot( std::ostream &os ) const {
    os << "RS";
}

Type *RessourceInst::out_type( int nout ) const {
    return gvm->type_Ressource;
}

Value make_RessourceInst() {
    return { new RessourceInst, 0, gvm->type_Ressource };
}

void RessourceInst::write_code( StreamSep &ss, Codegen &cg ) {
}
