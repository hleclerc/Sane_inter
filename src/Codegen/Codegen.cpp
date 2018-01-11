#include "Codegen.h"

Codegen::ReprType Codegen::repr( Type *type ) {
    return { this, type };
}

Codegen::ReprValue Codegen::repr( const Value &val, int prio, int flags ) {
    return { this, val, prio, flags };
}
