#include "Type_Union.h"
#include "Scope.h"
#include "Vm.h"

Type_Union::Type_Union( SI32 size, SI32 alig ) : Type( "union", size, alig ) {
}

void Type_Union::construct( Scope *scope, const Variable &self, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
}

void Type_Union::destroy( Scope *scope, const Variable &self, bool use_virtual ) {
}

bool Type_Union::has_a_constructor() const {
    return false;
}
