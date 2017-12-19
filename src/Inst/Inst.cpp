#include "Inst.h"

Inst::~Inst() {
}

void Inst::write_to_stream( std::ostream &os, SI32 nout, Type *type, int offset ) const {
    write_dot( os, nout, type, offset );
}
