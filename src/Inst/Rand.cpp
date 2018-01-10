#include "../Codegen/Codegen.h"
#include "../Type.h"
#include "Rand.h"

Rand::Rand( Type *type ) : type( type ) {
}

Rand::Rand( AttrClone, const Rand *inst ) : type( inst->type ) {
}

void Rand::write_dot( std::ostream &os ) const {
    os << "rand[" << *type << "]";
}

int Rand::nb_outputs() const {
    return 1;
}

Type *Rand::out_type( int nout ) const {
    return type;
}

Value make_Rand( Type *type ) {
    return { new Rand( type ), 0, type };
}

void Rand::write_inline_code( StreamPrio &ss, Codegen &cg ) {
    ss << "rand()";
}
