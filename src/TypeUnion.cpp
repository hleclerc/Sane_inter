#include "TypeUnion.h"
#include "Scope.h"
#include "Vm.h"

TypeUnion::TypeUnion( SI32 size, SI32 alig ) : Type( "union" ) {
    content.data.size = size;
    content.data.alig = alig;
}

void TypeUnion::construct( const Variable &self, const Vec<Variable> &args, const Vec<RcString> &names ) {
}

void TypeUnion::destroy( const Variable &self, bool use_virtual ) {
}

bool TypeUnion::has_a_constructor() const {
    return false;
}
