#include "../Type.h"
#include "../gvm.h"
#include "Void.h"

void Void::write_dot( std::ostream &os ) const {
    os << "void";
}

Value make_Void() {
    return make_Void( gvm->type_Void );
}

Value make_Void( Type *type ) {
    return { new Void, 0, type };
}
