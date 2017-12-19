#pragma once

#include "Type.h"
class Variable;

/**
*/
class TypeError : public Type {
public:
    TypeError();

    virtual void        destroy                    ( const Variable &self, bool use_virtual ) override;
    virtual void        spread_in                  ( const Variable &self, Vec<Variable> &res, Vec<RcString> &names ) override;
    virtual Variable    apply                      ( Variable &self, bool want_ret, const Vec<Variable> &args, const Vec<RcString> &names, const Variable &with_self, ApplyFlags apply_flags = ApplyFlags::NONE ) override;

    virtual bool        error                      () const override;

    virtual Variable    find_attribute             ( const RcString &name, const Variable &self, Variable::Flags flags, SI32 off ) const override;
    virtual Variable    with_self                  ( Variable &orig, const Variable &new_self ) const override;
};

