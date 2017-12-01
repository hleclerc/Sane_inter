#include "Type_Error.h"
#include "Scope.h"
#include "Vm.h"

Type_Error::Type_Error() : Type( "Error" ) {
}

void Type_Error::write_to_stream( std::ostream &os, const char *content ) const {
    os << "error";
}

Variable Type_Error::make_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) const {
    return scope->vm->ref_error;
}

Variable Type_Error::use_sl_trial( Scope *scope, bool want_ret, const Variable &func, const Variable &self, const Vec<Variable> &sl_args, const Vec<Rc_string> &sl_names, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor, const Variable &trial ) const {
    return scope->vm->ref_error;
}

void Type_Error::spread_in( Scope *scope, const Vec<Variable> &res ) {
}

Variable Type_Error::with_self( Scope *scope, const Variable &orig, const Variable &new_self ) {
    return scope->vm->ref_error;
}

void Type_Error::destroy( Scope *scope, const Variable &self, bool use_virtual ) {
}

Variable Type_Error::chbeba( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    return scope->vm->ref_error;
}

Variable Type_Error::select( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names ) {
    return scope->vm->ref_error;
}

Variable Type_Error::apply( Scope *scope, Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<Rc_string> &names, bool call_ctor ) {
    return scope->vm->ref_error;
}

bool Type_Error::error() const {
    return true;
}
