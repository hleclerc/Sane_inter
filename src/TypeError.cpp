#include "TypeError.h"
#include "gvm.h"

TypeError::TypeError() : Type( "Error" ) {
}

void TypeError::destroy( const Variable &self, bool use_virtual ) {
}

void TypeError::spread_in( const Variable &self, Vec<Variable> &res, Vec<RcString> &names ) {
}

Variable TypeError::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags ) {
    return gvm->ref_error;
}

bool TypeError::error() const {
    return true;
}

Variable TypeError::find_attribute( const RcString &name, const Variable &self , Variable::Flags flags, SI32 off ) const {
    return {};
}

Variable TypeError::with_self( Variable &orig, const Variable &new_self ) const {
    return gvm->ref_error;
}
