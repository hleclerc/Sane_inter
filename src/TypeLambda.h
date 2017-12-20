#pragma once

#include "Type.h"

/**
*/
class TypeLambda : public Type {
public:
    TypeLambda();

    virtual bool     destroy_attrs() const override;
    virtual Variable with_self    ( Variable &orig, const Variable &new_self ) const override;
    virtual Variable chbeba       ( Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {} ) override;
    virtual Variable select       ( Variable &self, bool want_ret, const Vec<Variable> &args = {}, const Vec<RcString> &names = {} ) override;
    virtual Variable apply        ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE ) override;
};

