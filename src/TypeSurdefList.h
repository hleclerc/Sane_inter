#pragma once

#include "Type.h"

/**
*/
class TypeSurdefList : public Type {
public:
    TypeSurdefList();

    virtual Variable with_self( Variable &orig, const Variable &new_self ) const override;
    virtual Variable apply    ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE ) override;
};

