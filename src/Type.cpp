#include "Variable.h"
#include "Type.h"

Type::Type( const LString &name ) {
    content.data.name = name;
}

void Type::write_to_stream( std::ostream &os ) const {
    os << content;
}

void Type::write_cst( std::ostream &os, const PI8 *data, int offset_mod_8 ) const {
    os << content;
}

void Type::destroy( const Variable &self, bool use_virtual ) {
}

void Type::spread_in( const Variable &self, Vec<Variable> &res, Vec<RcString> &names ) {
    TODO;
}

Variable Type::apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, ApplyFlags apply_flags ) {
    TODO; return {};
}

bool Type::error() const {
    return false;
}

bool Type::getsetter() const {
    return false;
}
