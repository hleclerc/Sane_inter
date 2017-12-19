#pragma once

#include "Type.h"

/**
*/
class TypeCallableWithSelf : public Type {
public:
    TypeCallableWithSelf();

    virtual Variable apply( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE );
};

