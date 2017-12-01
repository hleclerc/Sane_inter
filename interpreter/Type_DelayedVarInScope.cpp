#include "Type_DelayedVarInScope.h"
#include "System/rcast.h"
#include "Scope.h"
#include "Vm.h"

Type_DelayedVarInScope::Type_DelayedVarInScope() : Type_BaseBin( "DelayedVarInScope" ) {
}

Variable Type_DelayedVarInScope::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    DelayedVarInScope *div = rcast( self.ptr() );
    if ( div->value )
        return div->value;
    if ( scope )
        return scope->add_error( "variable should have been defined in a parent scope, but it has not been the case (early return ?)" ), scope->vm->ref_error;
    return scope->add_error( "variable '{}' is not defined yet in the scope" ), scope->vm->ref_error;
}

