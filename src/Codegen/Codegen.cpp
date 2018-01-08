#include "Codegen.h"

Codegen::ReprType Codegen::repr( Type *type ) {
    return { this, type };
}

Codegen::ReprValue Codegen::repr(const Value &val, int prio) {
    return { this, val, prio };
}