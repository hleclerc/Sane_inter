#include "System/rcast.h"
#include "Type_ScopeRef.h"
#include "Scope.h"
#include "Vm.h"

Type_ScopeRef::Type_ScopeRef() : Type_BaseBin( "ScopeRef" ) {
}

Variable Type_ScopeRef::find_attribute( Scope *scope, const Rc_string &name, const Variable *orig, GetSetter *gs, Variable::Content *content, Variable::Flags flags, SI32 off ) {
    Scope **n_scope = rcast( orig->ptr() );
    Variable res = (*n_scope)->find_variable( name, false );
    if ( ! res )
        return scope->add_error( "there's no variable '{}' in the mentionned scope", name ), scope->vm->ref_error;
    return res;
}
