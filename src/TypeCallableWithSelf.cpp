#include "TypeCallableWithSelf.h"
#include "CallableWithSelf.h"

TypeCallableWithSelf::TypeCallableWithSelf() : Type( "CallableWithSelf" ) {
}

Variable TypeCallableWithSelf::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) {
    CallableWithSelf *cs = self.rcast<CallableWithSelf>();
    return cs->callable.type->apply( cs->callable, want_ret, args, names, cs->self, apply_flags );
}
