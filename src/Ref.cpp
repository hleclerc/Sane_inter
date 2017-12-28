#include "Variable.h"

Ref::~Ref() {
}

Variable Ref::intercept_find_attribute( const RcString &name, Type *var_type, bool is_const, SI32 var_offset ) const {
    return {};
}

bool Ref::is_shared() const {
    return true;
}
