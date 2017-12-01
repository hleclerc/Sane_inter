#include "Type_CallableWithSelfAndArgs.h"
#include "System/rcast.h"
#include "Scope.h"
#include "Vm.h"

Type_CallableWithSelfAndArgs::Type_CallableWithSelfAndArgs() : Type_BaseBin( "CallableWithSelfAndArgs" ) {
}

Variable Type_CallableWithSelfAndArgs::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    CallableWithSelfAndArgs *c = rcast( self.ptr() );
    Vec<Variable> n_args;
    n_args << c->self;
    n_args.append( args );
    return c->callable.apply( scope, want_ret, n_args, names, call_ctor );
}
