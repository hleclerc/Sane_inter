#include "Variable.h"
#include "RefLeaf.h"

Variable::Variable( const RcPtr<Ref> &ref, Type *type, SI32 offset ) : ref( ref ), type( type ), offset( offset ) {
}

Variable::Variable( const Value &value ) : Variable( new RefLeaf( value ), value.type, 0 ) {
}

Variable &Variable::operator=( const Variable &value ) {
    ref    = value.ref;
    type   = value.type;
    offset = value.offset;
    return *this;
}

