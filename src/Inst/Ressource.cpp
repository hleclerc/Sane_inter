#include "Ressource.h"
#include "../gvm.h"

Ressource::Ressource() {
}

Ressource::Ressource( AttrClone, const Ressource *orig ) {
}

int Ressource::nb_outputs() const {
    return 1;
}

void Ressource::write_dot( std::ostream &os ) const {
    os << "RS";
}

Type *Ressource::out_type( int nout ) const {
    return gvm->type_Ressource;
}

Value make_Ressource() {
    return { new Ressource, 0, gvm->type_Ressource };
}

void Ressource::write_code( StreamSep &ss, Codegen &cg ) {
}
